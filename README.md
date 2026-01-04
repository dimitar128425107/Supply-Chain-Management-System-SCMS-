# SCMS – Simple Warehouse Management System

## Project overview
This is a console-based C program that simulates a small supply chain system.
Suppliers deliver items to a warehouse -> the warehouse sends items to stores.
The warehouse has a limited capacity.

## Core functionality
- Manage a warehouse with:
  - current quantity
  - maximum capacity
- Suppliers:
  - ID
  - maximum delivery per operation
- Stores:
  - ID
  - current quantity
- Operations:
  - Supplier → Warehouse delivery
  - Warehouse → Store delivery
  - Warehouse status screen
  - View-only settings (suppliers & stores)

  ## Key constraints
- IDs must be unique and positive
- Quantities must be positive
- Warehouse capacity cannot be exceeded
- Warehouse quantity cannot go below zero

## Planned future updates
- Full settings menu:
  - add / remove suppliers
  - add / remove stores
- History system:
  - keep last 5 deliveries from suppliers
  - keep last 5 transfers to stores
  - show history on warehouse screen
