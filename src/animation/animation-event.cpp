#include "../../include/animation/animation-event.hpp"

template <typename TypeUI>
void AnimationManager<TypeUI>::createAnimationEvent (std::unique_ptr<AnimationEvent<TypeUI>> event) {
    animationQueue.push(std::move(event));
    eventIDQueue.push(currentIndex);
}

template <typename TypeUI>
void AnimationManager<TypeUI>::nextStep() {
    currentIndex++;
}

template <typename TypeUI>
void AnimationManager<TypeUI>::popAnimation (TypeUI &ui) {
    while (eventIDQueue.size() && eventIDQueue.top() == displayIndex) {
        animationQueue.top()->apply(ui);
        animationQueue.pop();
        eventIDQueue.pop();
    }
    displayIndex++;
}