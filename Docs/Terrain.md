# Terrain System (M3)

## Data

- `TerrainAsset`: R32F heightmap (257–4097), 2× RGBA8 splatmaps (8 layers)
- Per-layer PBR material references, world scale
- On-disk: `.eterrain` binary + YAML metadata

## Rendering

- Quadtree chunks (64×64 quads), screen-space error LOD, skirts
- Compute normals from heightmap; splat-blended PBR; optional triplanar
- Dirty-chunk GPU updates only

## Brushes

raise / lower / smooth / flatten / set-height / paint-layer  
Undo via per-stroke dirty-rect Command snapshots.

## Physics

Jolt `HeightFieldShape`, regenerated per dirty chunk after stroke end.

## Scripting (runtime read-only in v1)

`Terrain.GetHeight(x, z)`, `Terrain.Raycast`
