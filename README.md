# Noteapp — Cross-platform C CLI Notes

Simple cross-platform command-line notes app written in C.

Build (CMake):

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Or build with Make (Linux/macOS with gcc) or with MinGW on Windows:

```bash
make
```

Run examples:

```bash
./noteapp add "Title"   # then type body on stdin and finish with Ctrl-D (Unix) or Ctrl-Z (Windows)
./noteapp list
./noteapp view <id>
./noteapp delete <id>
```

Notes are stored under the `notes/` directory inside the project.

License: Public domain (example project)

Web UI
------

A static web UI is included under the `web/` directory. It stores notes in the browser's `localStorage` and can import/export JSON.

To use the web UI, open `web/index.html` in your browser. For a better experience (avoid CORS/file issues), serve the `web/` folder with a simple HTTP server, for example:

```bash
# Python 3
cd web
python -m http.server 8000
# then open http://localhost:8000 in your browser
```

Vite-based Web App
------------------

A Vite-style demo is available under `web-vite/`. It is a minimal, dependency-free scaffold using native ES modules so you can run it with `vite` for a dev server or build. To try it:

```bash
cd web-vite
npm install
npm run dev
```

Then open the address printed by Vite (usually http://localhost:5173).


