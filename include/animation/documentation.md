# Guide for implementing animation handler

Since the calculations for animations happen almost immediately by the animation display is delayed so the user can view them clearly. Animations will be handled by `AnimationManager` and will be applied when it's the right time. Here is a brief explanation of the logic:

- `AnimationManager` stores a queue of animation events.
- A group of animation events belong to the same UI action is called an **animation step**, a group of animation steps belong to the same action is called an **UI action**.
- Every time an animation step is executed, the UI is copied and append to the history list with the corresponding action ID. Undo/redo operations are based on this history list, not the queue of animation events.