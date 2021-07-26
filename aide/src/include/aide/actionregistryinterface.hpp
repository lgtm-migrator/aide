
#ifndef AIDE_ACTIONREGISTRYINTERFACE_HPP
#define AIDE_ACTIONREGISTRYINTERFACE_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <QKeySequence>
#include <QList>

#include <aide/hierarchicalid.hpp>

class QAction;
class QKeySequence;

namespace aide
{
    class HierarchicalId;

    struct Action
    {
        std::weak_ptr<QAction> action;
        std::string description;
        QList<QKeySequence> defaultKeySequences;
        QList<QKeySequence> keySequences;

        QList<QKeySequence> getActiveKeySequences() const
        {
            return keySequences.empty() ? defaultKeySequences : keySequences;
        }
    };

    class ActionRegistryInterface
    {
    public:
        virtual ~ActionRegistryInterface() = default;

        virtual void registerAction(std::weak_ptr<QAction> action,
                                    const HierarchicalId& uniqueId) = 0;

        virtual void registerAction(std::weak_ptr<QAction> action,
                                    const HierarchicalId& uniqueId,
                                    std::string description) = 0;

        virtual void registerAction(
            std::weak_ptr<QAction> action, const HierarchicalId& uniqueId,
            const std::vector<QKeySequence>& defaultKeySequences) = 0;

        virtual void registerAction(
            std::weak_ptr<QAction> action, const HierarchicalId& uniqueId,
            std::string description,
            const std::vector<QKeySequence>& defaultKeySequences) = 0;

        virtual const std::map<HierarchicalId, Action>& actions() const = 0;
    };

    using ActionRegistryInterfacePtr = std::shared_ptr<ActionRegistryInterface>;
} // namespace aide

#endif // AIDE_ACTIONREGISTRYINTERFACE_HPP
