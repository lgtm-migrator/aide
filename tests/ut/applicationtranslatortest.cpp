#include <QApplication>

#include "applicationtranslator.hpp"
#include "catch2/catch.hpp"

using aide::gui::ApplicationTranslator;

TEST_CASE("ApplicationTranslator")
{
    int numberOfArgs{1};
    // NOLINTNEXTLINE
    std::array<char*, 1> appName{{const_cast<char*>("aide_test")}};
    QApplication app(numberOfArgs, appName.data());

    SECTION(" finds at least two languages by default")
    {
        ApplicationTranslator translator;

        REQUIRE(translator.getAvailableTranslations().size() >= 2);
    }

    SECTION(" finds german and english by default")
    {
        ApplicationTranslator translator;

        const std::set<std::string> availableTranslations{
            translator.getAvailableTranslations()};

        REQUIRE(availableTranslations.find("English (United States)") !=
                availableTranslations.end());
        REQUIRE(availableTranslations.find("German (Germany)") !=
                availableTranslations.end());
    }

    SECTION(" allows to translate a standard library string")
    {
        QLocale::setDefault(QLocale(QLocale::German));
        ApplicationTranslator translator;

        REQUIRE(QApplication::translate("MainWindow", "File") ==
                QString("Datei"));
    }
}
