#pragma once

#include <filesystem>
#include <unordered_map>

#include "Renderer/Texture.h"

class Dir {
public:
    Dir() = default;
    Dir(const std::string& path)
        : path(path)
    {
    }

    std::string path;
    std::vector<Dir> children;
};

class ContentBrowserPannel {
public:
    ContentBrowserPannel();
    ~ContentBrowserPannel() = default;

    void SetContext(std::filesystem::path ProjectDirectory) { m_ProjectDirectory = ProjectDirectory; };

    void DrawFoldersTree(const std::filesystem::path& root_path, unsigned int depth, int& index,
        int& selection);

    void OnImGuiRender();

private:
    std::filesystem::path m_ProjectDirectory = "assets";
    std::filesystem::path m_CurrentDirectory = "assets";

    int node_clicked = -1;

    std::unordered_map<std::string, Ref<HEngine::Texture2D>> m_Icons;
    std::unordered_map<std::string, Dir> dirs;

    float thumbnailSize = 100.f;
};
