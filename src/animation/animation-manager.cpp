#include "../../include/animation/animation-manager.hpp"

template <typename TypeUI, typename TypeLogic>
AnimationManager<TypeUI, TypeLogic>::AnimationManager (const TypeUI &init) :
    stateUI(1, init), completeUI(1, true), stateLogic(1), stateCode(1),
    currentEventStep(0), stateUIIterator(0), stateLogicIterator(0), stateCodeIterator(0) {

    // set position for UI and code highlighter
    stateUI[0].setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f});
    stateCode[0].setPosition({Setting::screenWidth - 20.f, Setting::screenHeight - 20.f});

    // initialize callback functions
    initCallbackFunctions();
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::createAnimationEvent (std::unique_ptr<AnimationEvent<TypeUI>> event) {
    animationQueue.push(std::move(event));
    eventIDQueue.push(currentEventStep);
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::nextStep() {
    currentEventStep++;
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::popAnimation() {
    if (eventIDQueue.empty()) return;

    TypeUI tempUI = getCurrentUI();
    UI::CodeHighlighter tempCode = getCurrentCode();
    bool changeTracker = false, completed = false;

    int currEvent = eventIDQueue.front();
    while (eventIDQueue.size() && eventIDQueue.front() == currEvent) {
        // std::cerr << "Popping Animation: " << typeid(*animationQueue.front()).name() << std::endl;
        int animationResult = animationQueue.front()->apply(tempUI, tempCode);
        if (animationResult == 1) // UI modified
            internalClock.restart(), changeTracker = true;
        else if (animationResult == 2) // animation completed
            completed = true;
        animationQueue.pop();
        eventIDQueue.pop();
    }

    if (changeTracker) {
        while (stateUIIterator + 1 < stateUI.size())
            stateUI.pop_back(), stateCode.pop_back(), completeUI.pop_back();
        stateUI.push_back(tempUI);
        stateCode.push_back(tempCode);
        completeUI.push_back(completed);
        stateUIIterator++, stateCodeIterator++;
    }
    else {
        getCurrentUI() = tempUI;
        getCurrentCode() = tempCode;
        completeUI.back() = completed;
    }
}

template <typename TypeUI, typename TypeLogic>
TypeUI& AnimationManager<TypeUI, TypeLogic>::getCurrentUI() {
    return stateUI[stateUIIterator];
}

template <typename TypeUI, typename TypeLogic>
TypeLogic& AnimationManager<TypeUI, TypeLogic>::getCurrentLogic() {
    return stateLogic[stateLogicIterator];
}

template <typename TypeUI, typename TypeLogic>
UI::CodeHighlighter& AnimationManager<TypeUI, TypeLogic>::getCurrentCode() {
    return stateCode[stateCodeIterator];
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::isComplete() {
    return completeUI[stateUIIterator];
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::previousState() {
    if (stateUIIterator == 0) return false;
    stateUIIterator--;
    getCurrentUI().fastForward();
    callbackEnableButtons(completeUI[stateUIIterator]);
    stateLogicIterator -= completeUI[stateUIIterator];
    return true;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::previousCompleteState() {
    bool lastModify = true;
    while (lastModify = previousState() && !isComplete());
    return lastModify;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::nextState() {
    if (stateUIIterator + 1 == stateUI.size()) return false;
    stateUIIterator++;
    getCurrentUI().fastForward();
    callbackEnableButtons(completeUI[stateUIIterator]);
    stateLogicIterator += completeUI[stateUIIterator];
    return true;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::nextCompleteState() {
    bool lastModify = true;
    while (lastModify = nextState() && !isComplete());
    return lastModify;
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::timePropagation (float deltaTime) {
    getCurrentUI().timePropagation(deltaTime);
    if (!internalClock.isFinished()) return;
    popAnimation();
    getCurrentUI().calculatePositions();
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::transformLogic (std::function<bool(TypeLogic&)> transformFunction) {
    TypeLogic newLogicVersion = getCurrentLogic();
    while (stateLogicIterator + 1 < stateLogic.size()) stateLogic.pop_back();
    if (transformFunction(newLogicVersion))
        stateLogic.push_back(newLogicVersion), stateLogicIterator++;
}

// ========== TEMPLATE-SPECIFIC FUNCTIONS: AnimationManager<UI::BinaryTree, DS::AVLTree> ==========

template<>
void AnimationManager<UI::BinaryTree, DS::AVLTree>::initCallbackFunctions() {
    stateLogic[0].setCallbackCreateNode([this] (int value, bool isRoot) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeCreateNode>(std::to_string(value), isRoot)
        );
    });
    stateLogic[0].setCallbackAddEdge([&] (int parent, int node, bool isLeft) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeAddEdge>(parent, node, isLeft)
        );
    });
    stateLogic[0].setCallbackChangeRoot([&] (int newRoot) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeChangeRoot>(newRoot)
        );
    });
    stateLogic[0].setCallbackDeleteNode([&] (int visualID) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeDeleteNode>(visualID)
        );
    });
    stateLogic[0].setCallbackSwapValue([&] (int a, int b) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeSwapValue>(a, b)
        );
    });
    stateLogic[0].setCallbackApplyAnimation([&]() {
        this->nextStep();
    });
    stateLogic[0].setCallbackHighlightNode([&] (int nodeID) {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeHighlightNode>(nodeID)
        );
    });
    stateLogic[0].setCallbackCompleteAnimation([&]() {
        this->createAnimationEvent(
            std::make_unique<BinaryTreeCompleteAnimation>()
        );
    });
    stateLogic[0].setCallbackLoadCode([&] (const std::vector<std::string> &vec) {
        this->createAnimationEvent(
            std::make_unique<CodeHighlightLoadCode<UI::BinaryTree>>(vec)
        );
    });
    stateLogic[0].setCallbackHighlightCode([&] (int row) {
        this->createAnimationEvent(
            std::make_unique<CodeHighlighting<UI::BinaryTree>>(row)
        );
    });
}