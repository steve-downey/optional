#!/usr/bin/env bash
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# Regenerate the [optional] wording from the specgen markup in
# include/beman/optional/optional.hpp.
#
#   SPECGEN       the specgen executable (default: specgen on PATH)
#   DRAFT_SOURCE  the `source` directory of a cplusplus/draft checkout; when
#                 set, the wording diff against the working draft is written too
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../.." && pwd)"
specgen="${SPECGEN:-specgen}"
header=include/beman/optional/optional.hpp
scratch="$(mktemp -d)"
trap 'rm -rf "$scratch"' EXIT

cd "$root"

render() { # render <backend> <output> [specgen options...]
    local backend="$1" out="$2"
    shift 2
    # A synopsis declares parameters without their names, as the draft's do.
    "$specgen" generate "$header" --backend "$backend" --no-compile-commands \
        --synopsis-parameter-names drop -o "$here/$out" "$@" -- -Iinclude \
        2>"$scratch/stderr" || { cat "$scratch/stderr" >&2; exit 1; }
}

# [optional] is a \rSec1 of the draft.
render latex wording.tex --validate --base-section-depth 1
render mpark wording.md

if [ -n "${DRAFT_SOURCE:-}" ]; then
    render latex diff-vs-draft.tex --base-section-depth 1 --baseline-draft "$DRAFT_SOURCE"
    render mpark diff-vs-draft.md --baseline-draft "$DRAFT_SOURCE"
fi

"$specgen" --version >"$here/specgen-version.txt"
