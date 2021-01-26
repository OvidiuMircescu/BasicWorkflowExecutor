# BasicWorkflowExecutor
Dynamic workflow traversal using events.

Each task submited to the executor creates a future which can be used to define
dependencies to for new tasks.
The executor guarantees the tasks are executed respecting the dependencies.
