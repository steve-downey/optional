#!/usr/bin/env bash
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# Regenerate the [optional.ref.monadic] wording from the specgen markup in
# include/beman/optional/optional.hpp.
#
#   SPECGEN       the specgen executable (default: specgen on PATH)
#   DRAFT_SOURCE  the `source` directory of a cplusplus/draft checkout; when
#                 set, the wording diff against the working draft is written too
#
# Only the monadic operations of optional<T&> carry markup so far, so the
# whole header is rendered with --split and this one fragment is kept.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
specgen="${SPECGEN:-specgen}"
header=include/beman/optional/optional.hpp
section=optional.ref.monadic
scratch="$(mktemp -d)"
trap 'rm -rf "$scratch"' EXIT

cd "$root"

render() { # render <backend> <extension> <output> [specgen options...]
    local backend="$1" ext="$2" out="$3"
    shift 3
    rm -rf "$scratch/split"
    "$specgen" generate "$header" --backend "$backend" --no-compile-commands \
        --split "$scratch/split" --root optional.root "$@" -- -Iinclude \
        >/dev/null 2>"$scratch/stderr" || { cat "$scratch/stderr" >&2; exit 1; }
    cp "$scratch/split/$section.$ext" "$here/$out"
}

render latex tex wording.tex
render mpark md wording.md

if [ -n "${DRAFT_SOURCE:-}" ]; then
    render latex tex diff-vs-draft.tex --baseline-draft "$DRAFT_SOURCE"
    render mpark md diff-vs-draft.md --baseline-draft "$DRAFT_SOURCE"
fi

"$specgen" --version >"$here/specgen-version.txt"
