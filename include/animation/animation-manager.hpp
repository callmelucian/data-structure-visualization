#pragma once
#include <SFML/System.hpp>
#include <queue>
#include <memory>
#include <functional>

// utilites
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"
#include "events.hpp"

// UI components
#include "../components/binary-tree.hpp"
#include "../components/code-highlighter.hpp"
#include "../components/graph.hpp"
#include "../components/hash-map.hpp"
#include "../components/linked-list.hpp"

// data structures
#include "../ds/avl-tree.hpp"
#include "../ds/dijkstra.hpp"
#include "../ds/red-black-tree.hpp"
#include "../ds/hash-map.hpp"
#include "../ds/linked-list.hpp"

template <typename TypeUI, typename TypeLogic>
class AnimationManager {
private:
    // animation queues
    std::queue<std::unique_ptr<AnimationEvent<TypeUI>>> animationQueue;
    std::queue<int> eventIDQueue;

    // objects with history manager
    std::vector<TypeUI> stateUI;
    std::vector<UI::CodeHighlighter> stateCode;
    std::vector<bool> completeUI;
    std::vector<TypeLogic> stateLogic;

    // iterators
    int currentEventStep, stateUIIterator, stateLogicIterator;
    bool isPlaying;
    
    // callback functions
    std::function<void(int)> callbackEnableButtons;
    std::function<void(bool)> callbackPlayPause;

    // countdown clock
    CountDownClock clock;

public:
    /**
     * @brief Assign a callback function that will be called
     * whenever Animation Manager need to enable/disable any interactions button
     */
    void setCallbackEnableButtons (auto func) {
        callbackEnableButtons = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever Animation Manager need to change the play/pause state
     */
    void setCallbackPlayPause (auto func) {
        callbackPlayPause = func;
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
     * @brief Return the current CodeHighlighter object
     */
    UI::CodeHighlighter& getCurrentCode();

    /**
     * @brief check whether the current UI is complete
     */
    bool isComplete();

    /**
     * @brief go back to the previous UI state
     */
    bool previousState (bool playing = false);

    /**
     * @brief go back to the previous complete UI state
     */
    bool previousCompleteState (bool playing = false);

    /**
     * @brief pass to the next UI state
     */
    bool nextState (bool playing = false);

    /**
     * @brief pass to the next complete UI stat
     */
    bool nextCompleteState (bool playing = false);

    /**
     * @brief time propagration
     */
    void timePropagation (float deltaTime);

    /**
     * @brief Apply transformation on logic structure
     */
    void transformLogic (std::function<bool(TypeLogic&)> transformFunction);

    void play();
    void pause();
    bool checkIsPlaying() const;

    /**
     * Initialize callback functions
     */
    void initCallbackFunctions();
};

template class AnimationManager<UI::BinaryTree, DS::AVLTree>;
template class AnimationManager<UI::Graph, DS::DijkstraAlgorithm>;
template class AnimationManager<UI::BinaryTree, DS::RedBlackTree>;
template class AnimationManager<UI::HashMap, DS::HashMap>;
template class AnimationManager<UI::LinkedList, DS::LinkedList>;