/***********************************************************************************************************************
*                                                                                                                      *
* glscopeclient                                                                                                        *
*                                                                                                                      *
* Copyright (c) 2012-2022 Andrew D. Zonenberg                                                                          *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

#include "ngscopeclient.h"
#include "FontManager.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction / destruction

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Font building

void FontManager::UpdateFonts(const PreferenceCategory& root)
{
	//Clear existing fonts, if any
	ImGuiIO& io = ImGui::GetIO();
	auto& atlas = io.Fonts;
	atlas->Clear();
	m_fonts.clear();

	//Add default Latin-1 glyph ranges plus some Greek letters and symbols we use
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
	builder.AddChar(L'°');

	//Build the range of glyphs we're using for the font
	ImVector<ImWchar> ranges;
	builder.BuildRanges(&ranges);

	//Make a list of fonts we want to load
	//TODO: get this from enumerating preferences
	set<FontDescription> fonts;
	fonts.emplace(FontDescription(FindDataFile("fonts/DejaVuSans.ttf"), 13));
	fonts.emplace(FontDescription(FindDataFile("fonts/DejaVuSansMono.ttf"), 13));
	fonts.emplace(FontDescription(FindDataFile("fonts/DejaVuSans-Bold.ttf"), 13));
	fonts.emplace(FontDescription(FindDataFile("fonts/DejaVuSans.ttf"), 20));

	//Load the fonts
	for(auto f : fonts)
		m_fonts[f] = atlas->AddFontFromFileTTF(f.first.c_str(), f.second, nullptr, ranges.Data);

	//Done loading fonts, build the texture
	atlas->Flags = ImFontAtlasFlags_NoMouseCursors;
	atlas->Build();
}