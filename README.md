# N-Body Stars (Planar Pleiades Problem)

Numerical simulation of a 7-body planar gravitational system, with trajectory visualization and minimum-distance (near-collision) analysis.

## Project Goal

This project solves the **Planar Pleiades problem** by:

- approximating the trajectories of 7 interacting stars on the time interval $[0, 3]$,
- plotting all trajectories on a single 2D graph,
- identifying the pair $(i, j)$ that reaches the minimum distance,
- determining the time at which this near-collision occurs.

## Mathematical Model

The governing system is:

```math

x_i''(t) = \sum_{j=1,\; j\neq i}^{7} m_j \frac{x_j(t)-x_i(t)}{r_{ij}^3}, \qquad
y_i''(t) = \sum_{j=1,\; j\neq i}^{7} m_j \frac{y_j(t)-y_i(t)}{r_{ij}^3}

```

with masses $m_i = i$, $i = 1, \dots, 7$, and

```math

r_{ij} = \sqrt{(x_i - x_j)^2 + (y_i - y_j)^2}.

```

In the implementation, the denominator is computed as $(dx^2 + dy^2)^{3/2}$, which is equivalent to $r_{ij}^3$.

## Initial Conditions

Initial positions:

- $x_1(0)=3,\; y_1(0)=3$
- $x_2(0)=3,\; y_2(0)=-3$
- $x_3(0)=-1,\; y_3(0)=2$
- $x_4(0)=-3,\; y_4(0)=0$
- $x_5(0)=2,\; y_5(0)=0$
- $x_6(0)=-2,\; y_6(0)=-4$
- $x_7(0)=2,\; y_7(0)=4$

Initial velocities:

- $x_i'(0)=0,\; y_i'(0)=0$ for $i=1,\dots,7$, except:
- $x_6'(0)=1.75$
- $x_7'(0)=-1.5$
- $y_4'(0)=-1.25$
- $y_5'(0)=1$

## Numerical Method

The second-order system is rewritten as a first-order system by introducing:

```math

v_{x_i} = x_i', \qquad v_{y_i} = y_i'.

```

Time integration is performed with the **4th-order Runge-Kutta method (RK4)** using:

- $T = 3$
- $\Delta t = 0.01$

## Repository Structure

- `main1.cpp`  
  Simulation entry point and near-collision output.
- `funzioni1.h`  
  Types, constants, and function declarations.
- `funzioni1.cpp`  
  Initialization, RK4 step, simulation loop, minimum-distance analysis.
- `Traiettorie.m`  
  MATLAB post-processing script for trajectory plotting and minimum-distance detection.

## Build

```bash
g++ -std=c++17 -O2 main1.cpp funzioni1.cpp -o nbody
```

## Run

```bash
./nbody
```

Generated output:

- `traiettorie_tempo.csv` with columns: `Time, Star, X, Y`

## MATLAB Post-Processing

In MATLAB:

```matlab
run('Traiettorie.m')
```

The script:

- plots all star trajectories in the plane,
- computes the global minimum distance across all star pairs,
- prints the involved pair and event time,
- highlights the minimum-distance segment on the plot.

## Numerical Results

From the simulation and trajectory analysis:

- **Minimum distance:** `0.0638515`
- **Star pair:** `3` and `5`
- **Event time:** `1.63`

These values indicate a near-collision event between stars 3 and 5 during system evolution.
