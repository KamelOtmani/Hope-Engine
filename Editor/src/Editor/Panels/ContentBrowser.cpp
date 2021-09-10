#include "ContentBrowser.h"

#include "HEngine.h"
#include "UI/ImGui_Custom.h"
#include "imgui.h"

#include <algorithm>

namespace fs = std::filesystem;

ContentBrowserPannel::ContentBrowserPannel()
{
    m_Icons["Folder"] = HEngine::Texture2D::Create("Ressources/Icons/Folder_filled.png");
    m_Icons["File"] = HEngine::Texture2D::Create("Ressources/Icons/File_filled.png");
};

void ContentBrowserPannel::DrawFoldersTree(const fs::path& root_path, unsigned int depth, int& index, int& selection)
{
    // Used the help from https://discourse.dearimgui.org/t/how-to-mix-imgui-treenode-and-filesystem-to-print-the-current-directory-recursively/37
    for (auto&& p : fs::directory_iterator(root_path)) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
            | ImGuiTreeNodeFlags_SpanFullWidth;
        if (fs::is_directory(p.path())) {
            auto leaf = 0;
            for (auto&& pp : fs::directory_iterator(p.path())) {
                if (pp.is_directory())
                    leaf++;
            }
            //ImGui::Text("Node :%d", node);
            auto leaf_flags = leaf > 0 ? node_flags : ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth;
            if (selection == index)
                leaf_flags |= ImGuiTreeNodeFlags_Selected;
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)index, leaf_flags, "%s", p.path().filename().string().c_str());
            if (ImGui::IsItemClicked()) {
                selection = index;
                m_CurrentDirectory = p.path();
            }
            if (node_open) {

                DrawFoldersTree(p, depth + 1, ++index, selection);
                ImGui::TreePop();
            }
        }
        ++index;
    }
    depth -= 1;
}
void ContentBrowserPannel::OnImGuiRender()
{

    ImGui::Begin("ContentBrowser");
    static float w = 200.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginChild("Tree Pannel", ImVec2(w, 0), true);

    int idx = 0u;
    DrawFoldersTree(m_ProjectDirectory, 0u, idx, node_clicked);

    ImGui::EndChild(); // Tree
    ImGui::SameLine();
    ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, ImGui::GetContentRegionAvail().y));
    if (ImGui::IsItemActive())
        w += ImGui::GetIO().MouseDelta.x;
    ImGui::PopStyleVar();

    ImGui::SameLine();

    ImGui::BeginGroup(); // Folders/Files Pannel
    ImGui::BeginChild("Header", { 0, 20 }); // Options
    if (m_CurrentDirectory != m_ProjectDirectory) {
        if (ImGui::Button("<-"))
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
    }
    ImGui::SameLine();
    ImGui::Text("This is a text");
    ImGui::EndChild(); // Options
    ImGui::Separator();

    auto pannelSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("MainPannel", { -1, pannelSize.y - 40 }); // Main Pannel

    float padding = 5.0f;
    float cellSize = thumbnailSize + padding;

    float pannelWidth = ImGui::GetContentRegionAvail().x;
    int cellCounts = std::max((int)(pannelWidth / cellSize), 1);

    ImGui::Columns(cellCounts, 0, false);

    std::vector<fs::path> paths;

    for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
        if (entry.exists()) {
            paths.push_back(entry);
        }
    }

    // sort by folder first
    std::stable_partition(paths.begin(), paths.end(), [&](fs::path v) { return fs::is_directory(v); });

    for (auto& entry : paths) {
        auto relativePath = std::filesystem::relative(entry, m_CurrentDirectory);
        auto entryString = entry.filename().string();
        if (fs::is_directory(entry)) {
            ImGui::ImageButton((ImTextureID)m_Icons["Folder"]->getID(), ImVec2(cellSize, cellSize), { 0, 1 }, { 1, 0 }, 1);
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                m_CurrentDirectory /= relativePath;
            }
            ImGui::TextWrapped(entryString.c_str());
        } else {
            ImGui::ImageButton((ImTextureID)m_Icons["File"]->getID(), ImVec2(cellSize, cellSize), { 0, 1 }, { 1, 0 }, 1);
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            }
            ImGui::TextWrapped(entryString.c_str());
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild(); // Main Pannel
    ImGui::Separator();
    ImGui::BeginChild("Options Pannel", { 0, 0 }); // Options
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 64, 512, "%.0f");
    ImGui::EndChild(); // Options

    ImGui::EndGroup(); // Folders/Files Pannel
    ImGui::End();
}
