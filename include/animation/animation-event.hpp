#include <queue>
#include <memory>

template <typename TypeUI>
class AnimationEvent {
private:
public:
    virtual ~AnimationEvent() = default;
    virtual void apply (TypeUI &ui) = 0;
};

template <typename TypeUI>
class AnimationManager {
private:
    std::queue<std::unique_ptr<AnimationEvent<TypeUI>>> animationQueue;
    std::vector<TypeUI> stateUI;
    std::queue<int> eventIDQueue;
    int currentEventStep, displayEventStep, currentAction, displayAction, stateIterator;
    std::function<void(bool)> callbackSetNextState, callbackSetPreviousState;

public:
    /**
     * @brief Assign a callback function that will be called
     * whenever Animation Manager need to enable/disable the next-state button
     */
    void setCallbackSetNextState (auto func) {
        setCallbackSetNextState = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever Animation Manager need to enable/disable the previous-state button
     */
    void setCallbackSetPreviousState (auto funct) {
        setCallbackSetPreviousState = func;
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
     * @brief go back to the previous UI state
     */
    void previousState();

    /**
     * @brief pass to the next UI state
     */
    void nextState();
};