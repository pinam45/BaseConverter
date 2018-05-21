/*****************************************************************************************
 *                                                                                       *
 * MIT License                                                                           *
 *                                                                                       *
 * Copyright (c) 2017 Maxime Pinard                                                      *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy          *
 * of this software and associated documentation files (the "Software"), to deal         *
 * in the Software without restriction, including without limitation the rights          *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell             *
 * copies of the Software, and to permit persons to whom the Software is                 *
 * furnished to do so, subject to the following conditions:                              *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all        *
 * copies or substantial portions of the Software.                                       *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR            *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,              *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE           *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,         *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE         *
 * SOFTWARE.                                                                             *
 *                                                                                       *
 *****************************************************************************************/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>
#include <cstring>

#include <imgui_easy_theming.hpp>
#include <ulltostr.hpp>
#include <clamp.hpp>

#define COUNT_OF(X) (sizeof(X)/sizeof(*(X)))

static const int minBase = 2;
static const int maxBase = 36;

struct TextFilters
{
	static int filterBase(ImGuiTextEditCallbackData* data)
	{
		if((data->EventChar >= '0' && data->EventChar < '0' + *(static_cast<int*>(data->UserData)))
		   || (data->EventChar >= 'A' && data->EventChar < 'A' + *(static_cast<int*>(data->UserData)) - 10))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
};

struct BaseBlock
{
	unsigned int base;
	char txt[128];

	explicit BaseBlock(unsigned int base_ = 10, const char* txt_ = "0")
	  : base(base_), txt()
	{
		std::strncpy(txt, txt_, 128);
	}
};

struct ThemeHolder
{
	const char name[12];
	const ImGuiColorTheme& colorTheme;
	bool enabled;
};

static void setupStyle();

void setupStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	ImGuiEasyTheming(ImGuiColorTheme::ArcDark);
}

int main()
{
	bool inputCustomBases = false;
	bool outputDefaultBases = true;
	bool outputCustomBases = false;

	int baseNb = 0;
	bool valUpd = false;
	bool baseUpd = false;
	bool tooBig = false;

	unsigned long long int value = 0;

	BaseBlock input{10};

	BaseBlock defaultBasesOut[]{
	  BaseBlock{10},
	  BaseBlock{16},
	  BaseBlock{8},
	  BaseBlock{2}
	};
	const char* defaultBasesOutDisplayTxt[]{
	  "DEC",
	  "HEX",
	  "OCT",
	  "BIN"
	};
	std::vector<BaseBlock> extraBasesOut{};

	std::vector<ThemeHolder> themeHolders{
	  ThemeHolder{"Arc Dark", ImGuiColorTheme::ArcDark, true},
	  ThemeHolder{"Flat UI", ImGuiColorTheme::FlatUI, false},
	  ThemeHolder{"Mint-Y-Dark", ImGuiColorTheme::MintYDark, false},
	};

	sf::Clock deltaClock;

	ImGui::GetIO().IniFilename = nullptr; // disable .ini saving
	setupStyle();

	sf::RenderWindow window(sf::VideoMode(300, 480), "Base converter");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	errno = 0;
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	while(window.isOpen())
	{
		sf::Event event{};
		while(window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::SetNextWindowSize(ImVec2{
		  static_cast<float>(window.getSize().x),
		  static_cast<float>(window.getSize().y)
		});
		ImGui::Begin("Main", nullptr,
		             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("Options"))
			{
				if(ImGui::BeginMenu("Color theme", true))
				{
					for(ThemeHolder& themeHolder : themeHolders)
					{
						if(ImGui::MenuItem(themeHolder.name, nullptr, themeHolder.enabled))
						{
							ImGuiEasyTheming(themeHolder.colorTheme);
							for(ThemeHolder& themeHolderBis : themeHolders)
							{
								themeHolderBis.enabled = false;
							}
							themeHolder.enabled = true;
						}
					}
					ImGui::EndMenu();
				}
				if(ImGui::MenuItem("Quit"))
				{
					ImGui::SFML::Shutdown();
					return EXIT_SUCCESS;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::PushItemWidth(-1);

		if(ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen |
		                                    ImGuiTreeNodeFlags_Leaf))
		{
			if(inputCustomBases)
			{
				ImGui::PushItemWidth(-40);
				valUpd |= ImGui::InputText("Value##IN", input.txt, sizeof(input.txt),
				                           ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank |
				                           ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter,
				                           TextFilters::filterBase, &input.base);
				int base = static_cast<int>(input.base);
				baseUpd |= ImGui::DragInt("Base", &base, 1, minBase, maxBase, "%.0f");
				input.base = static_cast<unsigned int>(clamp(base, minBase, maxBase));
				ImGui::PopItemWidth();
			}
			else
			{
				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() - 50);
				valUpd |= ImGui::InputText("##IN", input.txt, sizeof(input.txt),
				                           ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank |
				                           ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter,
				                           TextFilters::filterBase, &input.base);
				ImGui::PopItemWidth();
				ImGui::PushItemWidth(50);
				ImGui::SameLine();
				if(ImGui::Combo("##base", &baseNb, defaultBasesOutDisplayTxt, COUNT_OF(defaultBasesOutDisplayTxt)))
				{
					input.base = defaultBasesOut[baseNb].base;
					baseUpd = true;
				}
				ImGui::PopItemWidth();
			}
		}

		if(valUpd)
		{
			errno = 0;
			value = std::strtoull(input.txt, nullptr, static_cast<int>(input.base));
			tooBig = errno != 0;

			if(tooBig)
			{
				for(BaseBlock& baseBlock : defaultBasesOut)
				{
					baseBlock.txt[0] = '\0';
				}
				for(BaseBlock& baseBlock : extraBasesOut)
				{
					baseBlock.txt[0] = '\0';
				}
			}
			else
			{
				for(BaseBlock& baseBlock : defaultBasesOut)
				{
					if(!ulltostr(value, baseBlock.base, baseBlock.txt, sizeof(baseBlock.txt)))
					{
						baseBlock.txt[0] = '\0';
					}
				}
				for(BaseBlock& baseBlock : extraBasesOut)
				{
					if(!ulltostr(value, baseBlock.base, baseBlock.txt, sizeof(baseBlock.txt)))
					{
						baseBlock.txt[0] = '\0';
					}
				}
			}
			valUpd = false;
		}

		if(baseUpd)
		{
			if(!ulltostr(value, input.base, input.txt, sizeof(input.txt)))
			{
				input.txt[0] = '0';
				input.txt[1] = '\0';
				valUpd = true;
			}
			baseUpd = false;
		}

		if(tooBig)
		{
			ImGui::TextColored(ImColor(255, 0, 0), "Value is too large");
		}

		if(outputDefaultBases)
		{
			const char* defaultOutputTxt = outputCustomBases ? "Default output" : "Output";
			if(ImGui::CollapsingHeader(defaultOutputTxt,
			                           ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen |
			                           ImGuiTreeNodeFlags_Leaf))
			{
				ImGui::PushItemWidth(-30);
				for(size_t i = 0; i < COUNT_OF(defaultBasesOut); ++i)
				{
					ImGui::InputText(defaultBasesOutDisplayTxt[i], defaultBasesOut[i].txt,
					                 sizeof(defaultBasesOut[i].txt),
					                 ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);
				}
				ImGui::PopItemWidth();
			}
		}

		if(outputCustomBases)
		{
			const char* customOutputTxt = outputDefaultBases ? "Custom output" : "Output";
			if(ImGui::CollapsingHeader(customOutputTxt,
			                           ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen |
			                           ImGuiTreeNodeFlags_Leaf))
			{
				for(BaseBlock& baseBlock : extraBasesOut)
				{
					ImGui::PushItemWidth(-40);
					ImGui::PushID(&baseBlock);
					ImGui::InputText("Value", baseBlock.txt, sizeof(baseBlock.txt),
					                 ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);

					int base = static_cast<int>(baseBlock.base);
					if(ImGui::DragInt("Base", &base, 1, minBase, maxBase, "%.0f"))
					{
						baseBlock.base = static_cast<unsigned int>(clamp(base, minBase, maxBase));
						if(!ulltostr(value, baseBlock.base, baseBlock.txt, sizeof(baseBlock.txt)))
						{
							baseBlock.txt[0] = '\0';
						}
					}
					ImGui::PopID();
					ImGui::PopItemWidth();
					ImGui::Separator();
				}
				if(ImGui::Button("Add", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 20)))
				{
					extraBasesOut.emplace_back(defaultBasesOut[0].base, defaultBasesOut[0].txt);
				}
				ImGui::SameLine();
				if(ImGui::Button("Remove", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 20)))
				{
					if(!extraBasesOut.empty())
					{
						extraBasesOut.pop_back();
					}
				}
			}
		}

		if(ImGui::CollapsingHeader("Config", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Input custom bases", &inputCustomBases);
			ImGui::Checkbox("Output default bases", &outputDefaultBases);
			ImGui::Checkbox("Output custom bases", &outputCustomBases);
		}

		ImGui::PopItemWidth();

		ImGui::End();
		window.clear();
		ImGui::Render();
		window.display();
	}

	ImGui::SFML::Shutdown();
	return EXIT_SUCCESS;
}
