#include "../../include/animation/animation-event.hpp"

template <typename TypeUI>
AnimationManager<TypeUI>::AnimationManager (const TypeUI &init) :
    stateUI(1, init), completeUI(1, true), currentEventStep(0), stateIterator(0) {

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

    TypeUI tmp = getCurrentUI();
    bool changeTracker = false, completed = false;

    int currEvent = eventIDQueue.front();
    while (eventIDQueue.size() && eventIDQueue.front() == currEvent) {
        // std::cerr << "Popping Animation: " << typeid(*animationQueue.front()).name() << std::endl;
        int animationResult = animationQueue.front()->apply(tmp);
        if (animationResult == 1) // UI modified
            internalClock.restart(), changeTracker = true;
        else if (animationResult == 2) // animation completed
            completed = true;
        animationQueue.pop();
        eventIDQueue.pop();
    }

    if (changeTracker) {
        while (stateIterator + 1 < stateUI.size())
            stateUI.pop_back(), completeUI.pop_back();
        stateUI.push_back(tmp);
        completeUI.push_back(completed);
        stateIterator++;
    }
    else getCurrentUI() = tmp, completeUI.back() = completed;
}

template<typename TypeUI>
TypeUI& AnimationManager<TypeUI>::getCurrentUI() {
    return stateUI[stateIterator];
}

template<typename TypeUI>
bool AnimationManager<TypeUI>::isComplete() {
    return completeUI[stateIterator];
}

template <typename TypeUI>
bool AnimationManager<TypeUI>::previousState() {
    if (stateIterator == 0) return false;
    stateIterator--;
    getCurrentUI().fastForward();
    callbackEnableButtons(completeUI[stateIterator] ? -2 : -1);
    return true;
}

template <typename TypeUI>
bool AnimationManager<TypeUI>::previousCompleteState() {
    bool lastModify = true;
    while (lastModify = previousState() && !isComplete());
    return lastModify;
}

template <typename TypeUI>
bool AnimationManager<TypeUI>::nextState() {
    if (stateIterator + 1 == stateUI.size()) return false;
    stateIterator++;
    getCurrentUI().fastForward();
    callbackEnableButtons(completeUI[stateIterator] ? 2 : 1);
    return true;
}

template <typename TypeUI>
bool AnimationManager<TypeUI>::nextCompleteState() {
    bool lastModify = true;
    while (lastModify = nextState() && !isComplete());
    return lastModify;
}

template <typename TypeUI>
void AnimationManager<TypeUI>::timePropagation (float deltaTime) {
    getCurrentUI().timePropagation(deltaTime);
    if (!internalClock.isFinished()) return;
    popAnimation();
    getCurrentUI().calculatePositions();
}