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
    std::queue<int> eventIDQueue;
    int currentIndex, displayIndex;

public:
    void createAnimationEvent (std::unique_ptr<AnimationEvent<TypeUI>> event);

    void nextStep();

    void popAnimation (TypeUI &ui);
};