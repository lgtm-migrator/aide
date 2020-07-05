
#ifndef AIDE_MAINWINDOW_HPP
#define AIDE_MAINWINDOW_HPP

#include <memory>

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
} // namespace Ui

class QIcon;
class QWidget;

namespace aide
{
    class ActionRegistry;

    namespace gui
    {
        class TranslatorInterface;

        class MainWindow : public QMainWindow
        {
        public:
            explicit MainWindow(std::shared_ptr<ActionRegistry> actionRegistry,
                                QWidget* parent = nullptr);
            ~MainWindow() override;
            MainWindow(const MainWindow&) = delete;
            MainWindow& operator=(const MainWindow&) = delete;
            MainWindow(const MainWindow&&)           = delete;
            MainWindow& operator=(const MainWindow&&) = delete;

            [[nodiscard]] std::shared_ptr<TranslatorInterface> translator()
                const;

        private:
            void registerActions(
                const std::shared_ptr<ActionRegistry>& actionRegistry);

            [[nodiscard]] static QIcon createIconFromTheme(
                const std::string& iconName);

            std::shared_ptr<TranslatorInterface> m_translator;
            std::unique_ptr<Ui::MainWindow> m_ui;
            std::shared_ptr<QAction> m_actionQuit;
            std::shared_ptr<QAction> m_actionAboutQt;
        };
    } // namespace gui

} // namespace aide

#endif // AIDE_MAINWINDOW_HPP