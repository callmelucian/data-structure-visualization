#include "../../include/animation/animation-event.hpp"

template <typename TypeUI>
AnimationManager<TypeUI>::AnimationManager (const TypeUI &init) :
    stateUI(1, init), currentEventStep(0), displayEventStep(0), stateIterator(0) {}

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
    while (eventIDQueue.size() && eventIDQueue.front() == displayEventStep) {
        animationQueue.front()->apply(getCurrentUI());
        animationQueue.pop();
        eventIDQueue.pop();
    }
    displayEventStep++;
}

template<typename TypeUI>
TypeUI& AnimationManager<TypeUI>::getCurrentUI() {
    return stateUI[stateIterator];
}

template <typename TypeUI>
void AnimationManager<TypeUI>::previousState() {
    if (stateIterator == 0)
        return setCallbackSetPreviousState(false);
    
    // stateUI[stateIterator].copyPosition(stateUI[stateIterator - 1])
    stateIterator--;
    if (stateIterator == 0)
        setCallbackSetPreviousState(false);
    if (stateIterator + 2 == stateUI.size())
        setCallbackSetNextState(true);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::nextState() {
    if (stateIterator + 1 == stateUI.size())
        return setCallbackSetNextState(false);
    
    // stateUI[stateIterator].copyPosition(stateUI[stateIterator + 1])
    stateIterator++;
    if (stateIterator == 1)
        setCallbackSetPreviousState(true);
    if (stateIterator + 1 == stateUI.size())
        setCallbackSetNextState(false);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::timePropagation() {
    if (clock.getElapsedTime().asSeconds() < Setting::animationDelay) return;
    clock.reset();
    popAnimation();
}