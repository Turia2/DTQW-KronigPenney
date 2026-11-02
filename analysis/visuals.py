import os
import matplotlib
from matplotlib.animation import FuncAnimation, PillowWriter
import h5py
import numpy as np
import matplotlib.pyplot as plt
import os
matplotlib.use('Agg')  # Use a non-interactive backend for saving figures

H5_PATH = "data/simulation.h5"
OUT_DIR = "data/visuals"
os.makedirs(OUT_DIR, exist_ok=True)

with h5py.File(H5_PATH, 'r') as f:
    data = f['prob'][:]
    save_every = int(f.attrs['save_every'])
    T = int(f.attrs['timesteps'])

n_frames, N = data.shape
x = np.arange(N)
steps = np.arange(n_frames) * save_every


fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(x, data[-1], color = "blue", lw = 1.5, label = 'P(x, T)')
ax.fill_between(x, data[-1], color = "blue", alpha = 0.2)
ax.set_xlabel('Position x')
ax.set_ylabel('Probability P(x, T)')
ax.set_xlim(0, N-1)
ax.legend()
fig.savefig('data/visuals/probability_distribution.png', dpi=150)
plt.close(fig)


fig, ax = plt.subplots(figsize=(10, 6))
line,  = ax.plot([], [], color = "blue", lw = 1.5)
title = ax.text(0.02, 0.95, '', transform=ax.transAxes, fontsize=12, verticalalignment='top')
ax.set_xlim(0, N-1)
ax.set_ylim(0, np.max(data))
ax.set_xlabel('Position x')
ax.set_ylabel('Probability P(x, t)')

def update(frame):
    line.set_data(x, data[frame])
    title.set_text(f'Time step: {frame*save_every}')
    return line, title


animation = FuncAnimation(fig, update, frames=N, interval=100, blit=True)
animation.save('data/visuals/probability_evolution.gif', writer=PillowWriter(fps=10))
plt.close(fig)
