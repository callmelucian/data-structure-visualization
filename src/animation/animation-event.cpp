#include "../../include/animation/animation-event.hpp"

template <typename TypeUI>
AnimationManager<TypeUI>::AnimationManager (const TypeUI &init) :
    stateUI(1, init), currentEventStep(0), displayEventStep(0), stateIterator(0) {

    // center UI
    stateUI[0].setPosition({
        Setting::screenWidth / 2.f,
        Setting::screenHeight / 2.f
    });
}

template <typename TypeUI>
void AnimationManager<TypeUI>::createAnimationEvent (std::unique_ptr<AnimationEvent<TypeUI>> event) {
    animationQueue.push(std::move(event));
    eventIDQueue.push(currentEventStep);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::nextStep() {
    currentEventStep++;
}

template <typename TypeUI>
void AnimationManager<TypeUI>::popAnimation() {
    if (eventIDQueue.empty()) return;

    int currEvent = eventIDQueue.front();
    while (eventIDQueue.size() && eventIDQueue.front() == currEvent) {
        // std::cerr << "Popping Animation: " << typeid(*animationQueue.front()).name() << std::endl;
        if (animationQueue.front()->apply(getCurrentUI())) internalClock.restart();
        animationQueue.pop();
        eventIDQueue.pop();
    }
}

template<typename TypeUI>
TypeUI& AnimationManager<TypeUI>::getCurrentUI() {
    return stateUI[stateIterator];
}

template <typename TypeUI>
void AnimationManager<TypeUI>::previousState() {
    if (stateIterator == 0)
        return callbackSetPreviousStateButton(false);
    
    // stateUI[stateIterator].copyPosition(stateUI[stateIterator - 1])
    stateIterator--;
    if (stateIterator == 0)
        callbackSetPreviousStateButton(false);
    if (stateIterator + 2 == stateUI.size())
        callbackSetNextStateButton(true);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::nextState() {
    if (stateIterator + 1 == stateUI.size())
        return callbackSetNextStateButton(false);
    
    // stateUI[stateIterator].copyPosition(stateUI[stateIterator + 1])
    stateIterator++;
    if (stateIterator == 1)
        callbackSetPreviousStateButton(true);
    if (stateIterator + 1 == stateUI.size())
        callbackSetNextStateButton(false);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::timePropagation (float deltaTime) {
    getCurrentUI().timePropagation(deltaTime);
    if (!internalClock.isFinished()) return;
    // std::cerr << "Got here, not returned" << std::endl;
    popAnimation();
    getCurrentUI().calculatePositions();
}