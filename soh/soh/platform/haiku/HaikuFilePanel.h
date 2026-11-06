// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Techniix (Ship of Harkinian Haiku port)
#pragma once

#ifdef __HAIKU__
#include <string>

namespace SohHaiku {

// Shows a native Tracker open panel filtered to N64 ROMs (.z64/.n64/.v64)
// and blocks until the user picks a file or closes the panel.
// Returns false if nothing was picked or the panel could not be shown.
bool OpenRomFilePanel(const std::string& startDir, std::string& outPath);

} // namespace SohHaiku
#endif
