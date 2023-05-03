#pragma once

/*!
*  @brief Begin a new GUI window.
*  @param name Name of the window.
*/
#define BEGIN_GUI(name, openPtr, flags) if(ImGui::Begin(name, openPtr, flags)) {

/*!
*  @brief Display a property table as a list of key-value pairs.
*  @param propTable Property table to display.
*  @note The property table is a map of strings.
*/
#define DISPLAY_PROPERTY_TABLE(propTable) for(auto& prop : propTable) { ImGui::Text(prop.first.c_str()); ImGui::SameLine(); ImGui::Text(prop.second.c_str()); }

/*!
*  @brief End the current GUI window.
*/
#define END_GUI ImGui::End(); }


/*!
*  @brief Begin a new GUI meu bar.
*/
#define BEGIN_MAIN_MENU_BAR if(ImGui::BeginMainMenuBar()) {

/*!
*  @brief End the current GUI main menu bar.
*/
#define END_MAIN_MENU_BAR ImGui::EndMainMenuBar(); }

/*!
*  @brief Begin a new GUI menu.
*  @param name Name of the menu.
*/
#define BEGIN_MENU(name) if(ImGui::BeginMenu(name)) {

/*!
*   @brief Add a new menu item.
*   @param name Name of the menu item.
*   @param shortcut Shortcut key for the menu item.
*   @warning The menu item must be followed by a block of code to be executed when the menu item is selected.
*/
#define ADD_MENU_ITEM(name, shortcut, action) if(ImGui::MenuItem(name, shortcut)) { action = !action;}

/*!
*  @brief End the current GUI menu.
*/
#define END_MENU ImGui::EndMenu(); }

/*!
*  @brief Begin a new GUI tab bar.
*  @param name Name of the tab bar.
*  @param numColumns Number of columns.
*/
#define BEGIN_TABLE(name, numColumns) ImGui::BeginTable(name, numColumns);

/*!
*   @brief Move to the next row in the table.
*/
#define MOVE_TO_NEXT_ROW ImGui::TableNextRow();

/*!
*   @brief Set the column data for a specific index.
*   @param column Column index.
*   @param data Data to display in the column.
*   @note The column index must be set before calling this macro.
*/
#define SET_COLUMN_DATA(column, data) ImGui::TableSetColumnIndex(column); ImGui::Text(data.c_str());

/*!
*  @brief End the current GUI tab bar.
*/
#define END_TABLE ImGui::EndTable();

#define BEGIN_TAB_BAR(name) if(ImGui::BeginTabBar(name, ImGuiWindowFlags_AlwaysAutoResize))
#define ADD_TAB_ITEM(name, openPtr) if(ImGui::BeginTabItem(name, openPtr, ImGuiTabItemFlags_None)) {
#define END_TAB_ITEM ImGui::EndTabItem(); }
#define END_TAB_BAR ImGui::EndTabBar();

#define DISPLAY_MODAL_WINDOW(dispalyFlag, obj, ctx) if(dispalyFlag) { dispalyFlag = obj->showModal(ctx); }
#define DISPLAY_WINDOW(dispalyFlag, obj, ctx) if(dispalyFlag) { obj->show(ctx); }

#define INIT_CONTEXT(ctx) ImGui::SetCurrentContext(ctx);

#define DISPLAY_ABOUT_MENU(displayFlag, name) if(displayFlag) { ImGui::OpenPopup(name); if(ImGui::BeginPopupModal(name, &displayFlag, ImGuiWindowFlags_AlwaysAutoResize)) {ImGui::Text("Version: %d.%d", VERSION_MAJOR, VERSION_MINOR); ImGui::Text("Build date: %s", __DATE__); ImGui::Text("Build time: %s", __TIME__); ImGui::Text("Build type: %s", PROJECT_TYPE); if(!displayFlag) ImGui::CloseCurrentPopup(); ImGui::EndPopup(); }}

#define DISPLAY_VECTOR_ELEMENTS_STR(vec, color) for(auto& vecElem : vec) { ImGui::PushStyleColor(ImGuiCol_Text, color(vecElem)); ImGui::Text("%s", vecElem.c_str()); ImGui::PopStyleColor();}
#define DISPLAY_MAP_ELEMENTS_STR(map) for(auto& mapElem : map) { ImGui::Text("%s", mapElem.first.c_str()); ImGui::SameLine(); ImGui::Text("%s", mapElem.second.c_str()); }

#define RIGHT_BAR_WIDTH 500.0f
#define LEFT_BAR_WIDTH 350.0f
#define TOP_BAR_HEIGHT ImGui::GetFrameHeight()
#define BOTTOM_BAR_HEIGHT 250.0f