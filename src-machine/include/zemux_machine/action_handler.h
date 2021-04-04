#ifndef ZEMUX_MACHINE__ACTION_HANDLER
#define ZEMUX_MACHINE__ACTION_HANDLER

#include <memory>

namespace zemux {

struct ActionData {
    uint32_t simpleValue;
    std::unique_ptr<void*> complexValue;
};

class ActionHandler {
public:

    virtual uint16_t getActionHiWord() {
        return 0;
    }

    virtual ActionData onAction(uint32_t action, ActionData input) {
    }

protected:

    constexpr ActionHandler() = default;
    virtual ~ActionHandler() = default;
};

}

#endif
