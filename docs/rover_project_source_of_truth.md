# Rover Project Source Of Truth

## Purpose

This file is the single source of truth for the rover project.

It should be used to capture:

- current project goals
- visual direction
- implementation decisions
- code structure
- features already implemented
- known technical issues
- next likely tasks

When continuing work later, this file should be treated as the main handoff document.

## Project Goal

Build a more realistic OpenGL rover using hierarchical modeling, based on the provided rover reference direction.

The rover should feel like:

- a rover, not a car
- mechanically layered
- built from visibly separate subsystems
- readable in code
- modular in implementation

The project should stay:

- consistent with the current codebase structure
- simple enough to maintain
- reusable in terms of geometry generation
- not overengineered

## Visual Direction

The intended rover design direction is:

- chunkier rover-style wheels
- visible support structures between body and wheels
- thin layered chassis instead of one large box body
- a top-deck platform carrying upper rover subsystems
- a recognizable upper sensor/head silhouette

The current upper-body direction has shifted toward a binocular-style sensor head:

- a wider mast/support base
- two cylindrical vertical stands
- two cone-shaped eye/sensor housings

This is a stylized direction, somewhat inspired by a Wall-E-like binocular sensor setup, but still mounted on a rover-style chassis.

## Current Codebase Structure

Important source areas:

- `src/main.cpp`: scene setup, shared mesh creation, input handling, render loop, terrain drawing, camera mode toggling, rover placement
- `src/Rover.cpp`: rover-level hierarchical assembly and drawing
- `src/Tire.cpp`: wheel-only drawing and wheel sub-hierarchy
- `src/generateObjects.cpp`: procedural primitive generation
- `src/Camera.cpp`: camera movement and view-matrix behavior
- `utils/utility_functions.cpp`: terrain-height sampling and texture loading

Important headers:

- `include/Rover.h`: rover interface and rover-owned shared geometry references
- `include/Tire.h`: tire interface
- `include/Camera.h`: camera movement modes and view behavior
- `include/generateObjects.h`: primitive generation declarations
- `include/vertexObject.h`: mesh/VAO wrapper, including textured-terrain VAO creation

Shader files:

- `shaders/vertexShader.vs`
- `shaders/fragmentShader.fs`

## Architecture And Implementation Technique

The project currently uses a simple but effective modular structure:

### 1. Shared primitive generation

Primitive meshes are generated once in `main.cpp`, uploaded to OpenGL, and reused across the scene.

Currently used primitive types:

- cube
- cylinder
- torus
- cone

This is important because the rover should be assembled from reusable geometry rather than duplicating custom mesh data per subsystem.

### 2. Hierarchical rover assembly

The rover is drawn as a hierarchy inside `Rover::Draw(...)`.

This means:

- the rover receives one parent/world transform
- each subsystem applies its own local transform relative to that
- child parts inherit the parent transform chain

This is the core implementation technique used for the rover model.

### 3. Separation of responsibility

The current division of responsibility is:

- `main.cpp` owns shared mesh creation, world placement, and render orchestration
- `Rover` owns rover-level structure
- `Tire` owns wheel-only structure
- `generateObjects` owns primitive creation
- `Camera` owns movement and view logic

This separation should continue unless the rover assembly becomes crowded enough to justify another class.

### 4. Material / shading split

The renderer currently supports two material paths inside the shared shader:

- flat-color materials for rover parts
- textured terrain materials for the heightmap terrain

Implementation approach:

- terrain vertices now carry UVs in addition to position and normals
- the fragment shader uses a `useTexture` switch
- terrain samples a diffuse texture
- rover parts continue using `objectColor`

## What The Project Originally Started With

Before the current rover upgrade, the model was much simpler:

- 4 wheels
- a simple cube-like body
- direct wheel placement near the body corners
- wheel spin based on movement
- no proper support arm structure
- no layered chassis

That was enough for an initial hierarchical-modeling prototype, but not enough for the current rover design direction.

## Implemented Features So Far

### 1. Tire redesign

The original wheel look was replaced with a chunkier rover-style wheel.

Current tire composition:

- outer rubber uses torus geometry
- inner rim face uses cylinder geometry
- central hub uses cylinder geometry

Important notes:

- the wheel remains encapsulated in the `Tire` class
- the torus is used as the main outer rubber body
- temporary extra tread blocks were removed because they created an unwanted lumped look

### 2. Torus generation fix

The torus generation had a bug in `src/generateObjects.cpp`.

The issue:

- the tube-angle step used the wrong variable

The fix:

- `j * tubeSegments` was corrected to `j * tubeStep`

Why this mattered:

- the incorrect angle caused malformed torus geometry
- fixing it made the tire surface render correctly

### 3. Tire proportion tuning

The tire proportions were tuned in `main.cpp`.

Current torus generation values:

- `mainRadius = 0.72f`
- `tubeRadius = 0.28f`

Why this was done:

- the tire rubber needed to visually wrap the rim more convincingly
- a thicker torus produced a more rover-like result

### 4. Support-arm structure

The direct old body-to-wheel connection was replaced with a support assembly per wheel.

Per wheel, the support structure currently includes:

- one vertical hanger
- one horizontal arm
- one wheel mount block
- one tire at the end

All of these structural parts currently use the shared cube mesh.

This changed the hierarchy from:

- `rover root -> wheel`

to:

- `rover root -> support assembly -> wheel`

### 5. Lower deck frame

The rover now has a lower structural chassis layer above the support anchors.

Current lower deck composition:

- one thin center plate
- two side rails
- front cross beam
- rear cross beam

Implementation technique:

- all lower deck parts are cube instances with different transforms

Important placement detail:

- the first version had a visible gap between support arms and lower deck
- this was corrected by aligning the lower deck bottom face with the support-arm anchor height

### 6. Top deck platform

A second chassis layer was added above the lower deck.

Current top deck composition:

- one thin main platform
- two riser blocks between lower deck and top deck

Why it was kept thin:

- the rover should read as a platform structure, not as a big rectangular body block
- the top deck should act as a mounting surface for upper subsystems

### 7. Mast base / head-support platform

An upper deck subsystem was added on top of the top deck.

This started as a smaller mast base and was later widened and thickened.

Current role:

- acts as the main support platform for the sensor head assembly

Why it was widened:

- the original small base did not visually support the planned binocular-style head
- the larger base makes the upper assembly feel more structurally believable

### 8. Dual cylindrical sensor stands

Two vertical cylindrical stands were added on top of the widened mast base.

Why:

- to create a binocular-style support structure
- to prepare mounting points for twin sensor eyes

Implementation detail:

- the shared cylinder mesh is reused
- the stands are drawn in `Rover::Draw(...)`

### 9. Cone-shaped eye housings

Two cone-shaped eye/sensor housings were added above the cylindrical stands.

Why:

- they create a clear upper silhouette
- they move the rover toward a more distinctive sensor-head design

Implementation detail:

- reusable cone geometry was added to `generateObjects`
- one shared cone mesh is created in `main.cpp`
- the shared cone VAO is passed into `Rover`
- `Rover` draws the eye housings as part of the rover hierarchy

### 10. Rover orientation adjustment

The whole rover hierarchy is rotated in `main.cpp` before drawing.

Why:

- to make the rover face the camera/viewing direction more appropriately
- this was done at the parent transform level so the entire hierarchy rotates together

### 11. Directional light and rover spotlight

The scene now uses two active light types:

- one directional light acting as the sun
- one rover-mounted spotlight acting as the lamp

Directional light behavior:

- provides broad scene lighting
- can be toggled on and off at runtime

Spotlight behavior:

- is mounted near the rear lamp assembly on the rover
- uses a cutoff and outer cutoff for a focused beam
- can be toggled on and off at runtime
- is positioned and aimed from rover-local coordinates transformed into world space

### 12. Lamp cone orientation fix

The rover lamp previously had a visual mismatch:

- the spotlight beam direction was functionally correct
- but the rendered cone mesh looked reversed relative to the emitted beam

The fix:

- the cone mesh orientation in `src/Rover.cpp` was flipped so the base of the cone visually faces along the beam direction

Result:

- the lamp now reads as emitting light from the base of the cone
- the visual cone orientation matches the apparent beam direction more closely

### 13. Terrain texture mapping

The terrain now supports first-pass texture mapping.

Current implementation:

- terrain vertices now include UV coordinates
- a textured terrain VAO path was added for position/normal/UV vertex layouts
- the terrain uses `utils/rock_ground_diff_4k.jpg` as a diffuse texture
- terrain UVs are tiled across the heightmap grid using a fixed tiling factor
- the shared shader can now render both textured and untextured materials

This means the terrain now has:

- geometry driven by `utils/iceland_heightmap.png`
- appearance driven by `utils/rock_ground_diff_4k.jpg`

### 14. Two camera modes

The project now supports two different camera behaviors.

#### Ground-view camera

This is the terrain-following walking camera.

Behavior:

- uses `W/A/S/D` movement along the world plane
- camera `y` is recomputed from terrain height
- the user visually feels terrain bumps while moving

Why:

- it simulates movement close to the ground
- it is useful for experiencing the terrain surface directly

#### Top-down free camera

This is the free-flowing top-view camera.

Behavior:

- uses `W/A/S/D` to move across the scene
- uses `SPACE` and `LEFT_SHIFT` to move up and down
- does not snap `y` to terrain height
- looks straight downward at the terrain

Why:

- it allows overhead inspection
- it avoids the ground-bump feeling
- it is useful for scene debugging and layout inspection

#### Camera toggle

The user can toggle camera modes with:

- `C`

Implementation detail:

- the toggle is handled in `main.cpp`
- camera mode state is stored in `Camera`
- terrain-following only runs in ground mode
- top-down free movement only runs in top-down mode

### 15. Rover yaw rotation control

The rover can now be rotated manually around the world `Y` axis at runtime.

Behavior:

- `Q` rotates the rover left
- `E` rotates the rover right
- the rotation is stored as persistent rover yaw state
- the full rover hierarchy rotates together because the yaw is applied at the parent transform level in `main.cpp`

Implementation detail:

- `Rover` now owns `yawDegrees` and `rotationSpeed`
- the yaw is updated in `Rover::Update(...)`
- `main.cpp` combines the original rover-facing offset with the user-controlled yaw before drawing
- the rover-mounted spotlight also rotates correctly because its position and direction are derived from the same parent transform

### 16. Cone side texture-seam fix

The cone side UV mapping was corrected to avoid the most obvious side-seam artifact.

The issue:

- the side surface previously wrapped from `u` near `1.0` directly back to `u = 0.0` in the closing triangle
- that caused the texture to interpolate across the full image width inside one triangle
- this made the side texture look visibly broken or stretched at the seam

The fix:

- the side of the cone now creates an extra closing vertex column
- the duplicated closing column uses `u = 1.0`
- side indices now connect consecutive vertices instead of wrapping directly with modulo for the side seam

Result:

- the texture wraps around the cone side more cleanly
- the seam is materially smoother
- some texture compression near the tip still remains, which is expected for a cone because the surface collapses to a point

## Current Hierarchical Structure

The rover is currently organized conceptually like this:

- rover root transform
- lower deck frame
- top deck platform
- mast base / head-support platform
- two cylindrical sensor stands
- two cone-shaped eye housings
- rear lamp stand
- rear lamp cone
- four support-arm assemblies

Each support-arm assembly contains:

- vertical support
- horizontal arm
- wheel mount
- tire

The tire itself is hierarchical:

- outer torus rubber
- left rim face
- right rim face
- central hub

## Camera Design Notes

### Why ground mode feels terrain bumps

In ground mode, camera height is sampled from the terrain every frame:

- terrain height is looked up using camera `x/z`
- camera `y` is then overwritten

So the camera is always being attached to the terrain surface.

### Why top-down mode does not feel terrain bumps

In top-down mode, terrain-following is disabled.

That means:

- the terrain no longer controls camera `y`
- the camera can move independently in 3D space

This is the key reason the free camera does not feel the bumps.

### Why the top-down camera looks perpendicularly downward

In top-down mode, the view target is directly below the camera along the negative `Y` axis.

That means the camera is looking perpendicular to the terrain plane.

## Primitive Usage Summary

Current primitive responsibilities:

- cube: support arms, lower deck, top deck, mast base, risers, structural blocks
- cylinder: wheel rims, wheel hub, sensor stands
- torus: outer tire rubber
- cone: eye/sensor housings

This primitive reuse is intentional and should continue when possible.

Additional note:

- the cone primitive is also reused for the rover lamp housing

## Terrain And Material System

### Heightmap terrain

The terrain is still generated from the grayscale heightmap in `utils/iceland_heightmap.png`.

The heightmap is responsible for:

- terrain shape
- vertex height values
- terrain normals used for lighting

### Terrain diffuse texture

The terrain now also uses a diffuse color texture:

- `utils/rock_ground_diff_4k.jpg`

This texture is not used to shape the terrain.
It is used only to color the terrain surface.

So the terrain now has a clear split:

- heightmap controls geometry
- diffuse texture controls appearance

### Terrain UV mapping

Terrain vertices now store:

- position
- normal
- UV coordinates

The UVs are generated procedurally from the terrain grid coordinates in `src/generateObjects.cpp`.

To avoid stretching the texture across the full terrain only once, the UVs are tiled across the grid.

Current implementation detail:

- terrain texture tiling factor is `12.0f`

### Textured terrain rendering path

The terrain uses a separate VAO creation path for vertices containing:

- 3 floats for position
- 3 floats for normal
- 2 floats for texture coordinates

The shader path now supports:

- `diffuseTexture` sampler
- `useTexture` boolean switch

Rendering behavior:

- terrain enables `useTexture`
- rover disables `useTexture` and continues using flat color

This keeps the terrain texturing work isolated without forcing all procedural meshes onto a textured material path.

## Controls

Current controls:

- `W/A/S/D`: camera horizontal movement
- `SPACE`: move camera upward in top-down mode
- `LEFT_SHIFT`: move camera downward in top-down mode
- `C`: toggle between ground-view and top-down-view camera
- arrow keys: rover movement
- `Q`: rotate rover counterclockwise around the `Y` axis
- `E`: rotate rover clockwise around the `Y` axis
- `1`: toggle directional light
- `2`: toggle rover spotlight

## Known Technical Notes

- The rover terrain placement logic in `main.cpp` still uses rover position before `rover.Update(...)`; this should be corrected later
- The camera system currently supports a top-down free camera and a ground-follow camera, but there is not yet a forward-tilted overhead camera mode
- The current binocular sensor head could still use more mechanical connection detail
- Terrain texturing currently uses one diffuse image only; there is not yet slope-based blending, detail mapping, or normal mapping
- The shared shader now supports both textured and untextured materials, which is practical for now but may eventually justify a cleaner material split
- The cone seam is improved, but textured cones can still show expected pinching near the apex because a pointed surface compresses UV space

## Build Verification

Current status:

- the project builds successfully with `msbuild Learning.sln /t:Build /p:Configuration=Debug /p:Platform=x64`

Remaining warnings are currently limited to:

- numeric conversion warnings already present in parts of the codebase
- GLFW PDB/linker warnings caused by missing debug symbols for the prebuilt GLFW library

## Current Strengths Of The Implementation

- clear modular separation
- shared geometry reuse
- rover hierarchy is easy to extend
- wheel logic is still isolated inside `Tire`
- camera logic is becoming more structured rather than ad hoc
- lighting is now more readable because the rover has both a global sun and a local lamp
- the terrain now has materially better visual richness due to tiled diffuse texturing

## Current Weaknesses / Refinement Opportunities

- rover proportions may still need tuning
- upper sensor assembly may look too disconnected without a bridge or lens detail
- rover placement on terrain should use the updated rover position
- the current camera is functional, but still simple and axis-aligned
- terrain texturing is still only first-pass and may show visible repetition depending on camera distance
- terrain shading would improve further with multi-texture blending based on slope or height

## Recommended Next Technical Steps

Possible next work items:

1. refine the binocular sensor head
2. add lens details to the cone eyes
3. add a bridge or bar connecting the two eye supports
4. refine rover wheel/support proportions
5. add deck-mounted equipment blocks
6. fix rover terrain placement timing in `main.cpp`
7. improve terrain material quality with slope-based texture blending
8. add a third camera style later if needed, such as an angled overhead camera

## Recommended Next Conversation Starting Points

When continuing later, a good next instruction could be one of these:

- "Refine the binocular-style sensor head and make the two eyes feel more mechanically connected."
- "Tune the rover proportions so the upper body and wheel system feel more balanced."
- "Fix the rover terrain-follow placement bug in main.cpp."
- "Add another camera mode with a tilted overhead view."
