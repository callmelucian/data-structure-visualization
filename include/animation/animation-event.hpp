#pragma once
#include <SFML/System.hpp>
#include <queue>
#include <memory>
#include <functional>

#include "../core/global-setting.hpp"
#include "../components/binary-tree.hpp"
#include "../core/utility.hpp"
#include "../ds/avl-tree.hpp"

#include "binary-tree-animation.hpp"

template <typename TypeUI, typename TypeLogic>
class AnimationManager {
private:
    std::queue<std::unique_ptr<AnimationEvent<TypeUI>>> animationQueue;
    std::queue<int> eventIDQueue;
    std::vector<TypeUI> stateUI;
    std::vector<bool> completeUI;
    std::vector<TypeLogic> stateLogic;
    int currentEventStep, stateUIIterator, stateLogicIterator;
    std::function<void(int)> callbackEnableButtons;
    CountDownClock<Setting::animationDelay> internalClock;

public:
    /**
     * @brief Assign a callback function that will be called
     * whenever Animation Manager need to enable/disable any interactions button
     */
    void setCallbackEnableButtons (auto func) {
        callbackEnableButtons = func;
    }

    /**
     * @brief constructor
     */
    AnimationManager (const TypeUI &init);

    /**
     * @brief create an animation event with a corresponding event ID
     */
    void createAnimationEvent (std::unique_ptr<AnimationEvent<TypeUI>> event);

    /**
     * @brief use next event ID
     */
    void nextStep();

    /**
     * @brief apply all animation steps with the next displaying event ID
     */
    void popAnimation();

    /**
     * @brief get the current UI object
     */
    TypeUI& getCurrentUI();

    /**
     * @brief get the current logic object
     */
    TypeLogic& getCurrentLogic();

    /**
     * @brief check whether the current UI is complete
     */
    bool isComplete();

    /**
     * @brief go back to the previous UI state
     */
    bool previousState();

    /**
     * @brief go back to the previous complete UI state
     */
    bool previousCompleteState();

    /**
     * @brief pass to the next UI state
     */
    bool nextState();

    /**
     * @brief pass to the next complete UI stat
     */
    bool nextCompleteState();

    /**
     * @brief time propagration
     */
    void timePropagation (float deltaTime);

    /**
     * @brief Apply transformation on logic structure
     */
    void transformLogic (std::function<bool(TypeLogic&)> transformFunction);

    /**
     * Initialize callback functions
     */
    void initCallbackFunctions();
};

template class AnimationManager<UI::BinaryTree, DS::AVLTree>;