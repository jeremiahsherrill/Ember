---
name: ember-automation
description: Add editor Commands and expose them through the Ember JSON-RPC automation API.
---

# Ember Automation Skill

## Rule

Every editor mutation is a `Command`. UI, undo/redo, `ember-cli`, and headless tests all execute Commands.

## Adding a feature

1. Implement `class MyCommand : public Command` with `Execute` / `Undo`
2. Register in the Command factory (string name → creator)
3. Wire UI button to `CommandHistory::Execute(CreateRef<MyCommand>(...))`
4. Add JSON-RPC method in AutomationServer that constructs the same Command
5. Document in `Docs/AutomationAPI.md`
6. Add Catch2 or headless test

## Client

```bash
ember-cli --port 7777 <method> [args...]
```
