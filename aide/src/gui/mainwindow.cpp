
#include "mainwindow.hpp"

#include <utility>

#include <QMessageBox>
#include <QObject>
#include <QPushButton>

#include <aide/logger.hpp>

#include "../core/actionregistry.hpp"
#include "applicationtranslator.hpp"
#include "hierarchicalid.hpp"
#include "mainwindowcontroller.hpp"
#include "ui_mainwindow.h"

using aide::HierarchicalId;
using aide::core::MainWindowInterface;
using aide::gui::MainWindow;
using aide::gui::MainWindowControllerPtr;
using aide::gui::TranslatorInterface;

struct ActionIds
{
    const HierarchicalId MAIN_MENU_FILE_QUIT{
        HierarchicalId("Main Menu")("File")("Quit")};
    const HierarchicalId MAIN_MENU_HELP_ABOUT_QT{
        HierarchicalId("Main Menu")("Help")("About Qt")};
};

const static ActionIds& ACTION_IDS()
{
    try {
        static ActionIds ids;
        return ids;
    }
    catch (...) {
        AIDE_LOG_CRITICAL(
            "MainWindow: could not create hierarchical menu Ids with static "
            "storage duration. This should only happen if the application is "
            "already out of memory at startup")
        std::terminate();
    }
}

MainWindow::MainWindow(const ActionRegistryInterfacePtr& actionRegistry,
                       QWidget* parent)
    : MainWindowInterface(parent)
    , m_translator{new ApplicationTranslator}
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    registerActions(actionRegistry);
}

MainWindow::~MainWindow() = default;

void MainWindow::setMainWindowController(MainWindowControllerPtr controller)
{
    m_controller = std::move(controller);
}

void MainWindow::registerActions(
    const ActionRegistryInterfacePtr& actionRegistry)
{
    m_actionQuit =
        std::make_shared<QAction>(createIconFromTheme("application-exit"),
                                  QApplication::tr("Quit"), this);
    connect(m_actionQuit.get(), &QAction::triggered, QApplication::instance(),
            &QApplication::quit);
    m_ui->menuFile->addAction(m_actionQuit.get());

    actionRegistry->registerAction(
        m_actionQuit, ACTION_IDS().MAIN_MENU_FILE_QUIT,
        QApplication::tr("Quits the application").toStdString(),
        {QKeySequence(QKeySequence::Quit), QKeySequence("Alt+F4")});

    m_actionAboutQt =
        std::make_shared<QAction>(QApplication::tr("About Qt"), this);
    connect(m_actionAboutQt.get(), &QAction::triggered,
            QApplication::instance(), &QApplication::aboutQt);
    m_ui->menuHelp->addAction(m_actionAboutQt.get());

    actionRegistry->registerAction(m_actionAboutQt,
                                   ACTION_IDS().MAIN_MENU_HELP_ABOUT_QT);
}

std::shared_ptr<TranslatorInterface> MainWindow::translator() const
{
    return m_translator;
}

QIcon MainWindow::createIconFromTheme(const std::string& iconName)
{
    QIcon icon;
    QString iconThemeName = QString::fromStdString(iconName);
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon = QIcon::fromTheme(iconThemeName);
    } else {
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
    }
    return icon;
}

void MainWindow::closeEvent([[maybe_unused]] QCloseEvent* event)
{
    AIDE_LOG_TRACE("User requested to close application")
    m_controller->onUserWantsToQuitApplication(event);
}

bool MainWindow::letUserConfirmApplicationClose()
{
    AIDE_LOG_DEBUG("Asking user for confirmation to close application")
    auto messageBox = std::make_unique<QMessageBox>(this);

    const auto reply = messageBox->question(
        this, tr("Confirm exit"), tr("Are you sure you want to exit?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    return reply == QMessageBox::Yes;
}
