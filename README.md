# Look Out (Legacy)

> ⚠️ **Archived.** This is the old Unreal Engine 5 codebase for Look Out. It is no longer maintained and is kept for historical reference only. The project has been rebuilt from scratch in Unity in definitely not near future.

## About

Look Out is a sandbox/simulation game project. This repository contains the first prototype, built in UE5 before I scrapped it and started over.

## What was implemented

- Grabbing, interacting with objects
- Working (sort of) inventory

## Why it was scrapped

The code ended up poorly structured:

- Tightly coupled systems with no clear module boundaries
- Logic scattered across Blueprints and C++ without a consistent approach
- Hard to extend or refactor without breaking other parts

Rather than patching it, I decided to rewrite from scratch with a cleaner architecture.

## Lessons learned

- Design the architecture and system boundaries before writing gameplay code (who would've thought)
- Keep responsibilities separated and dependencies explicit
- Decide early on a C++/Blueprint split and stick to it (or don't even try to work with UE)
- Don't let a prototype quietly turn into the production codebase
