#include "../../include/animation/animation-manager.hpp"

template <typename TypeUI, typename TypeLogic>
AnimationManager<TypeUI, TypeLogic>::AnimationManager (const TypeUI &init) :
    stateUI(1, init), completeUI(1, true), stateLogic(1), stateCode(1),
    currentEventStep(0), stateUIIterator(0), stateLogicIterator(0), isPlaying(false) {

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

    TypeUI tempUI = (isPlaying ? stateUI.back() : getCurrentUI());
    UI::CodeHighlighter tempCode = (isPlaying ? stateCode.back() : getCurrentCode());
    bool changeTracker = false, completed = false;

    int currEvent = eventIDQueue.front();
    while (eventIDQueue.size() && eventIDQueue.front() == currEvent) {
        int animationResult = animationQueue.front()->apply(tempUI, tempCode);
        if (animationResult == 1) changeTracker = true; // UI modified
        else if (animationResult == 2) completed = true; // animation completed
        animationQueue.pop();
        eventIDQueue.pop();
    }

    if (changeTracker) {
        while (!isPlaying && stateUIIterator + 1 < stateUI.size())
            stateUI.pop_back(), stateCode.pop_back(), completeUI.pop_back();
        tempUI.calculatePositions();
        stateUI.push_back(tempUI);
        stateCode.push_back(tempCode);
        completeUI.push_back(completed);
    }
    else {
        (isPlaying ? stateUI.back() : getCurrentUI()) = tempUI;
        (isPlaying ? stateCode.back() : getCurrentCode()) = tempCode;
        if (isPlaying) completeUI.back() = completed;
        else completeUI[stateUIIterator] = completed;
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
    return stateCode[stateUIIterator];
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::isComplete() {
    return completeUI[stateUIIterator];
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::previousState() {
    pause();
    if (stateUIIterator == 0) return false;
    TypeUI tempUI = getCurrentUI();
    stateUIIterator--;
    getCurrentUI().copyPosition(tempUI);
    if (!isPlaying) callbackEnableButtons(completeUI[stateUIIterator]);
    stateLogicIterator -= completeUI[stateUIIterator];
    return true;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::previousCompleteState() {
    pause();
    TypeUI tempUI = getCurrentUI();
    bool lastModify = true;
    while (lastModify = previousState() && !isComplete());
    getCurrentUI().copyPosition(tempUI);
    return lastModify;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::nextState() {
    pause();
    if (stateUIIterator + 1 == stateUI.size()) return false;
    TypeUI tempUI = getCurrentUI();
    stateUIIterator++;
    getCurrentUI().copyPosition(tempUI);
    if (!isPlaying)
        callbackEnableButtons(completeUI[stateUIIterator]);
    stateLogicIterator += completeUI[stateUIIterator];
    return true;
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::nextCompleteState() {
    pause();
    TypeUI tempUI = getCurrentUI();
    bool lastModify = true;
    while (lastModify = nextState() && !isComplete());
    getCurrentUI().copyPosition(tempUI);
    return lastModify;
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::timePropagation (float deltaTime) {
    while (eventIDQueue.size()) popAnimation();
    if (isPlaying && internalClock.isFinished()) {
        if (stateUIIterator + 1 == stateUI.size()) pause();
        else {
            nextState();
            internalClock.restart();
            play();
        }
    }
    getCurrentUI().timePropagation(deltaTime);
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::transformLogic (std::function<bool(TypeLogic&)> transformFunction) {
    TypeLogic newLogicVersion = (isPlaying ? stateLogic.back() : getCurrentLogic());
    while (!isPlaying && stateLogicIterator + 1 < stateLogic.size()) stateLogic.pop_back();
    while (!isPlaying && stateUIIterator + 1 < stateUI.size())
        stateUI.pop_back(), stateCode.pop_back(), completeUI.pop_back();
    if (transformFunction(newLogicVersion)) stateLogic.push_back(newLogicVersion);
    play();
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::play() {
    isPlaying = true;
    callbackPlayPause(true);
}

template <typename TypeUI, typename TypeLogic>
void AnimationManager<TypeUI, TypeLogic>::pause() {
    isPlaying = false;
    callbackPlayPause(false);
}

template <typename TypeUI, typename TypeLogic>
bool AnimationManager<TypeUI, TypeLogic>::checkIsPlaying() const {
    return isPlaying;
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
            std::make_unique<CompleteAnimation<UI::BinaryTree>>()
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

// ========== TEMPLATE-SPECIFIC FUNCTIONS: AnimationManager<UI::Graph, DS::Dijkstra> ==========

template<>
void AnimationManager<UI::Graph, DS::DijkstraAlgorithm>::initCallbackFunctions() {
    stateLogic[0].setCallbackCreateNode([&] (int value) {
        this->createAnimationEvent(
            std::make_unique<GraphCreateNode>(value)
        );
    });
    stateLogic[0].setCallbackAddEdge([&] (int fromNode, int toNode, int weight) {
        this->createAnimationEvent(
            std::make_unique<GraphAddEdge>(fromNode, toNode, weight)
        );
    });
    stateLogic[0].setCallbackDeleteNode([&] (int nodeID) {
        this->createAnimationEvent(
            std::make_unique<GraphDeleteNode>(nodeID)
        );
    });
    stateLogic[0].setCallbackDeleteEdge([&] (int edgeID) {
        this->createAnimationEvent(
            std::make_unique<GraphDeleteEdge>(edgeID)
        );
    });
    stateLogic[0].setCallbackEditEdge([&] (int edgeID, int newWeight) {
        this->createAnimationEvent(
            std::make_unique<GraphEditEdge>(edgeID, newWeight)
        );
    });
    stateLogic[0].setCallbackClearAnnotation([&]() {
        this->createAnimationEvent(
            std::make_unique<GraphClearAnnotation>()
        );
    });
    stateLogic[0].setCallbackEditAnnotation([&] (int nodeID, int value) {
        this->createAnimationEvent(
            std::make_unique<GraphEditAnnotation>(nodeID, value)
        );
    });
    stateLogic[0].setCallbackMarkAnnotation([&] (int nodeID) {
        this->createAnimationEvent(
            std::make_unique<GraphMarkAnnotation>(nodeID)
        );
    });
    stateLogic[0].setCallbackHighlightNode([&] (int nodeID) {
        this->createAnimationEvent(
            std::make_unique<GraphHighlightNode>(nodeID)
        );
    });
    stateLogic[0].setCallbackHighlightEdge([&] (int edgeID) {
        this->createAnimationEvent(
            std::make_unique<GraphHighlightEdge>(edgeID)
        );
    });
    stateLogic[0].setCallbackSetEdgeColor([&] (int edgeID, sf::Color color) {
        this->createAnimationEvent(
            std::make_unique<GraphSetEdgeColor>(edgeID, color)
        );
    });
    stateLogic[0].setCallbackApplyAnimation([&]() {
        this->nextStep();
    });
    stateLogic[0].setCallbackCompleteAnimation([&]() {
        this->createAnimationEvent(
            std::make_unique<CompleteAnimation<UI::Graph>>()
        );
    });
    stateLogic[0].setCallbackLoadCode([&] (const std::vector<std::string> &code) {
        this->createAnimationEvent(
            std::make_unique<CodeHighlightLoadCode<UI::Graph>>(code)
        );
    });
    stateLogic[0].setCallbackHighlightCode([&] (int row) {
        this->createAnimationEvent(
            std::make_unique<CodeHighlighting<UI::Graph>>(row)
        );
    });

    stateUI[0].setCallbackTriggerAddEdge([&] (int fromNode, int toNode) {
        this->transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            return logic.createEdge(fromNode, toNode), true;
        });
    });
}
