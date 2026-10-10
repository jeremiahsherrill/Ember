# Automation API

Status: **stub in M0** · full JSON-RPC in **M2**.

## Launch

```bash
Ember-Editor --automation-port 7777 [--headless]
ember-cli --port 7777 scene.describe
```

## Protocol

JSON-RPC 2.0 over TCP localhost.

## Planned methods

| Method | Purpose |
|--------|---------|
| `scene.new` / `scene.save` / `scene.load` / `scene.describe` | Scene lifecycle |
| `entity.create` / `entity.destroy` / `entity.duplicate` | Entities |
| `component.add` / `component.remove` / `property.get` / `property.set` | Components |
| `asset.import` / `asset.list` | Assets |
| `terrain.sculpt` / `terrain.paint` | Terrain tools |
| `viewport.screenshot` | Agent visual feedback |
| `play.start` / `play.stop` | Play mode |
| `project.export` | Distribution builds |
| `command.undo` / `command.redo` | History |
| `test.run` | Headless test project |

Every method maps to one or more `Command` objects.
