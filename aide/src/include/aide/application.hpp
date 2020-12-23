
#ifndef AIDE_APPLICATION_HPP
#define AIDE_APPLICATION_HPP

#include <memory>

#include <QApplication>

#include <aide/logger.hpp>

#include "../core/aidesettingsprovider.hpp"
#include "../core/applicationclose.hpp"
#include "../gui/mainwindowcontroller.hpp"

class QMainWindow;

namespace aide
{
    class ActionRegistry;
    namespace gui
    {
        class MainWindow;
        class TranslatorInterface;
    } // namespace gui

    /**
     * @brief The aide::Application class manages the GUI application
     *
     * This class is derived from QApplication and is the main entry point
     * of the library. Therefore it can be used in the same way as a normal
     * QApplication
     *
     * It creates and manages the main window and main settings.
     *
     */
    class Application : public QApplication
    {
    public:
        // NOLINTNEXTLINE
        explicit Application(int& argc, char* argv[]);

        [[nodiscard]] std::shared_ptr<Logger> logger() const;

        [[nodiscard]] std::shared_ptr<QMainWindow> mainWindow() const;

        [[nodiscard]] std::shared_ptr<gui::TranslatorInterface> translator()
            const;

    private:
        static bool isOrganizationNameSet();

        static std::shared_ptr<aide::Logger> setupLogger();

        [[nodiscard]] static bool tryToCreateLogLocationIfItDoesNotExist(
            const QString& logLocation);

        std::shared_ptr<aide::Logger> m_logger{setupLogger()};

        std::shared_ptr<aide::ActionRegistry> m_actionRegistry;

        std::shared_ptr<aide::gui::MainWindow> m_mainWindow;

        AideSettingsProvider settingsProvider;

        aide::core::ApplicationClose m_applicationClose;

        aide::gui::MainWindowControllerPtr m_mainController;
    };
} // namespace aide
#endif // AIDE_APPLICATION_HPP
