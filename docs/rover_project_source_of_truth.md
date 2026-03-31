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
- `utils/utility_functions.cpp`: terrain-height sampling

Important headers:

- `include/Rover.h`: rover interface and rover-owned shared geometry references
- `include/Tire.h`: tire interface
- `include/Camera.h`: camera movement modes and view behavior
- `include/generateObjects.h`: primitive generation declarations
- `include/vertexObject.h`: mesh/VAO wrapper

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

### 11. Two camera modes

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

## Current Hierarchical Structure

The rover is currently organized conceptually like this:

- rover root transform
- lower deck frame
- top deck platform
- mast base / head-support platform
- two cylindrical sensor stands
- two cone-shaped eye housings
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

## Controls

Current controls:

- `W/A/S/D`: camera horizontal movement
- `SPACE`: move camera upward in top-down mode
- `LEFT_SHIFT`: move camera downward in top-down mode
- `C`: toggle between ground-view and top-down-view camera
- arrow keys: rover movement

## Known Technical Notes

- Full build verification was previously blocked by a filesystem permission issue in the Visual Studio debug `tlog` path
- The rover terrain placement logic in `main.cpp` still uses rover position before `rover.Update(...)`; this should be corrected later
- The camera system currently supports a top-down free camera and a ground-follow camera, but there is not yet a forward-tilted overhead camera mode
- The current binocular sensor head could still use more mechanical connection detail

## Current Strengths Of The Implementation

- clear modular separation
- shared geometry reuse
- rover hierarchy is easy to extend
- wheel logic is still isolated inside `Tire`
- camera logic is becoming more structured rather than ad hoc

## Current Weaknesses / Refinement Opportunities

- rover proportions may still need tuning
- upper sensor assembly may look too disconnected without a bridge or lens detail
- rover placement on terrain should use the updated rover position
- the current camera is functional, but still simple and axis-aligned

## Recommended Next Technical Steps

Possible next work items:

1. refine the binocular sensor head
2. add lens details to the cone eyes
3. add a bridge or bar connecting the two eye supports
4. refine rover wheel/support proportions
5. add deck-mounted equipment blocks
6. fix rover terrain placement timing in `main.cpp`
7. add a third camera style later if needed, such as an angled overhead camera

## Recommended Next Conversation Starting Points

When continuing later, a good next instruction could be one of these:

- "Refine the binocular-style sensor head and make the two eyes feel more mechanically connected."
- "Tune the rover proportions so the upper body and wheel system feel more balanced."
- "Fix the rover terrain-follow placement bug in main.cpp."
- "Add another camera mode with a tilted overhead view."
