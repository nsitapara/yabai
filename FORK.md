# nsitapara/yabai

This branch is [Christian-SC26/yabai](https://github.com/Christian-SC26/yabai)
v7.1.39 plus the fixes our desk needs. Our setup runs macOS 27 with SIP enabled
and no scripting addition, so spaces switch through the Dock gesture path.
Upstream is [asmvik/yabai](https://github.com/asmvik/yabai). Christian-SC26 is
upstream dd84572 (v7.1.25) plus 88 commits.

The work lives on branch `macos27-combined`. To take a new Christian-SC26
release, merge `christian/master` into it and rerun the checks below.

## What this branch adds

The first five are ports of the patches the dotfiles build has been running
since September and October 2026 (`~/dotfiles/yabai-patches/`). The rest are
new and have only been built and checked offline. See each commit for details.

| Commit | Change | Status |
|---|---|---|
| fix(query) | `--displays` and `--spaces` close their arrays when there are no displays or a space is skipped | ported |
| fix(window) ordered-out | Untile windows their app hides on close (Electron, ChatGPT), tile them again on reopen | ported, plus new focus guards for the fork's front-switch code |
| fix(window) reopened | Replace a known window when its app reopens it with a new element (Spotify, ChatGPT) | ported |
| fix(mouse) | A click during a retile no longer rewrites splits or swaps windows | ported, rebased onto the fork's mouse code |
| fix(space) | No rate limit on user space switches; refuse removed spaces; post all gesture phases or none; report gesture failures | ported core, guard removal new |
| fix(display) DISPLAY_CHANGED | display_changed fires again when yabai focuses a display itself | new |
| fix(display) re-probe | NULL checks for displays that vanish during wake | new |
| fix(window) busy app | Retry a window whose app was blocked when it opened (keychain prompts) | new |
| fix(mission-control) | Run the delayed focus restore on the event loop, and never restore to a removed space | new |

## Build

```sh
make install          # serial: make -j races on the shared osax recipe
```

If a parallel build left `src/osax/payload_bin.c` or `loader_bin.c` empty,
delete both files and build again. `make install` runs the icon step, which adds
resource-fork attributes. Run `xattr -cr` on the copied binary or app bundle
before an ad-hoc `codesign --sign -`. Do not use `make sign`, `make archive`,
`scripts/install.sh` or `scripts/release.sh`, which publish or install
Christian-SC26's release.

`yabai --version` still prints the fork's version (v7.1.39), so check the
binary's path or the commit to tell this build apart.

## Tests

```sh
tests/local/run.sh    # safe: compiles real sources against stubs, posts no events
make -C tests build   # compile only
```

Do not run `tests/bin/tests`, `make -C tests`, `make -C tests run`,
`tests/legacy` or `tests/test_rapid_switch.py` on a desktop in use. They post
real gestures, clicks and window moves. Run them only in a throwaway VM.

## Live checks before relying on a build

- Cmd+1 → 2 → 3 → 2 pressed quickly, then jumps 4 → 1 and 6 → 1. Every press lands.
- Cmd+number to the other display's space, then Alt+Tab back. `display --focus next`
  fires display_changed.
- Close and reopen ChatGPT and Spotify. No gap is left, and the tile comes back.
- Hold the mouse on a tiled window while another window opens beside it, then
  release. The split does not jump.
- `./wm.sh spaces` with a surplus desktop active.
- Launch Pilot with its keychain prompts showing. Its window tiles after the prompts.
- Sleep and wake with the desk monitors. yabai keeps running.

## Not changed yet (decide after a live probe)

- The fork's Mission Control heuristic, which scans Dock and WindowManager
  windows at layers 16-19 (`mission_control_is_active`). If those windows appear
  outside Mission Control, revert to upstream's mode check. Move the 1204
  registration back at the same time.
- The fork's `window_manager_focus_window_with_raise` rewrite. Revert to
  upstream if Ctrl+Tab or Cmd+arrow leaves keyboard focus behind.
- `mouse_bsp_live_resize`, which now defaults to on. Set it in yabairc to
  keep classic alt-drag resizing.
- The 20f1a82 resize path: a failed live resize can leave a window untiled
  until the next retile.
