#pragma once
#include <SFML/System.hpp>
#include <queue>
#include <memory>
#include <functional>

#include "../core/global-setting.hpp"
#include "../components/binary-tree.hpp"
#include "../core/utility.hpp"

template <typename TypeUI>
class AnimationEvent {
private:
public:
    virtual ~AnimationEvent() = default;
    virtual bool apply (TypeUI &ui) = 0;
};

template <typename TypeUI>
class AnimationManager {
private:
    std::queue<std::unique_ptr<AnimationEvent<TypeUI>>> animationQueue;
    std::queue<int> eventIDQueue;
    std::vector<TypeUI> stateUI;
    std::vector<bool> completeUI;
    int currentEventStep, stateIterator;
    std::function<void(bool)> callbackEnableButtons;
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
     * @brief get the current UI to draw
     */
    TypeUI& getCurrentUI();

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
     * @brief mark the last UI state as complete
     */
    void completeAnimation();
};

template class AnimationManager<UI::BinaryTree>;