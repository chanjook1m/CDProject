#ifndef _IMGUI_H
#define _IMGUI_H

#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include <filesystem>
#include "imfilebrowser.h"
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <math.h>

#include "imgui_dock.h"
#include "implot.h"
#include "implot_internal.h"

//#include "implot_demo.h"

#include "Utils/OBJLoader.h"
#include "Simulation/SimulationModel.h"
#include "Utils/FileSystem.h"
#include "Simulation/Simulation.h"
#include "Simulation/DistanceFieldCollisionDetection.h"

#define BIT(x) (1 << x)
#define PI std::atan(1.0)*4

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	GLFWwindow* m_window;
	ImGui::FileBrowser m_fileDialog;
	ImVec4 m_clear_color;
	ImGuiIO* m_io;
	int m_w, m_h;
	std::string m_dirPath;

	int m_filetreeitem_current_idx;
	std::string m_filetree_current_item;
	std::string m_filetree_double_clicked_item;
	std::vector<std::string> m_filetreeitems;
	unsigned int m_filetree_num;

	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;
	unsigned int m_fbo;
	GLuint m_shaderProgram;

	std::map<std::string, int> m_map_filetree_rigidbody;
	static int m_key_count;

	Eigen::Vector3f translation;

	/// ImPlot
	struct ScrollingBuffer {
		int MaxSize;
		int Offset;
		ImVector<ImVec2> Data;
		ScrollingBuffer(int max_size = 2000) {
			MaxSize = max_size;
			Offset = 0;
			Data.reserve(MaxSize);
		}
		void AddPoint(float x, float y) {
			if (Data.size() < MaxSize)
				Data.push_back(ImVec2(x, y));
			else {
				Data[Offset] = ImVec2(x, y);
				Offset = (Offset + 1) % MaxSize;
			}
		}
		void Erase() {
			if (Data.size() > 0) {
				Data.shrink(0);
				Offset = 0;
			}
		}
	};

	// utility structure for realtime plot
	struct RollingBuffer {
		float Span;
		ImVector<ImVec2> Data;
		RollingBuffer() {
			Span = 10.0f;
			Data.reserve(2000);
		}
		void AddPoint(float x, float y) {
			float xmod = fmodf(x, Span);
			if (!Data.empty() && xmod < Data.back().x)
				Data.shrink(0);
			Data.push_back(ImVec2(xmod, y));
		}
	};

public:

	void Initialize(GLFWwindow* window);
	void StartFrame();
	void Render();
	void Cleanup();
	void ItemRowsBackground(float lineHeight, const ImColor& color);
	std::pair<bool, uint32_t> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask);
	void showDirectoryTree(std::string directoryPath);
	void loadObj(const std::string& filename, PBD::VertexData& vd, Utilities::IndexedFaceMesh& mesh, const Vector3r& scale);
	void addRigidbody(std::string fName);
	
	// reset ��ư ������ ȣ��
	void reset(); 

	void createMainMenuBar();
	void createLeftSideMenu();
	void createRightSideMenu();
	void createCenterMenu();
	void createBottomMenu();
	void createFileDialogBtn();
	

	void fbo_init();
	void fbo_bind();
	void fbo_unbind();
	void fbo_cleanup();
	void draw();
};
#endif