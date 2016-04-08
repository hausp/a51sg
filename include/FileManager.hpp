/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Point.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

class FileManager {
public:
	typedef struct {
		std::string name;
		std::vector<std::string> args;
	} command;

	template<unsigned D>
	std::string toObj(const std::vector<Drawable<D>*>& shapes) {
		std::vector<Point<D>> vertices;
		std::stringstream definitions;

		for (auto& shape : shapes) {
			auto pointList = shape->points();
			std::vector<unsigned> vertexIndices;
			vertexIndices.reserve(pointList.size());
			for (auto point : pointList) {
				unsigned numVertices = vertices.size();
				bool found = false;
				for (unsigned i = 0; i < numVertices; i++) {
					if (vertices[i] == point) {
						vertexIndices.push_back(i);
						found = true;
						break;
					}
				}
				if (!found) {
					vertexIndices.push_back(numVertices);
					vertices.push_back(point);
				}
			}

			definitions << "o " << shape->getName() << std::endl;
			if (pointList.size() == 1) {
				definitions << "p " << vertexIndices.back() + 1 << std::endl;
			} else {
				definitions << "l ";
				for (auto index : vertexIndices) {
					definitions << index + 1 << " ";
				}
				definitions << std::endl;
			}
		}

		std::stringstream result;
		for (auto vertex : vertices) {
			result << "v " << vertex[0] << " " << vertex[1] << " 0" << std::endl;
		}
		result << definitions.str();
		return result.str();
	}

	std::vector<Drawable<2>*> fromObj2D(const std::string& filename) {
		return fromObj<2>(filename);
	}

	std::vector<Drawable<3>*> fromObj3D(const std::string& filename) {
		return fromObj<3>(filename);
	}

private:

	template<unsigned D>
	std::vector<Drawable<D>*> fromObj(const std::string& filename) {
		auto commands = parse(filename);
		std::vector<Drawable<D>*> displayFile;
		std::vector<Point<D>> vertices;
		std::string currShapeName;
		for (auto& cmd : commands) {
			if (cmd.name == "v") {
				Point<D> v;
				for (unsigned i = 0; i < D; i++) {
					v[i] = stod(cmd.args[i]);
				}
				vertices.push_back(v);
			} else if (cmd.name == "o") {
				currShapeName = cmd.args[0];
			} else if (cmd.name == "p") {
				auto point = new Point<D>(vertices[stoi(cmd.args[0]) - 1]);
				point->setName(currShapeName);
				displayFile.push_back(point);
			} else if (cmd.name == "l") {
				std::vector<Point<D>> points;
				for (auto arg : cmd.args) {
					points.push_back(vertices[stoi(arg) - 1]);
				}

				if (cmd.args.size() > 2) {
					auto polygon = new Polygon<D>(points);
					polygon->setName(currShapeName);
					displayFile.push_back(polygon);
				} else {
					auto line = new Line<D>(currShapeName, points[0], points[1]);
					displayFile.push_back(line);
				}
			}
		}
		return displayFile;
	}

	std::vector<command> parse(const std::string& filename) {
		std::ifstream input(filename);
		std::string line;
		std::vector<command> commandList;
		while (std::getline(input, line)) {
			if (line.size() == 0 || line[0] == '#') {
				continue;
			}

			std::istringstream stream(line);
			std::string command;
			std::vector<std::string> args;
			std::string arg;

			stream >> command;
			while (stream >> arg) {
				args.push_back(arg);
			}
			commandList.push_back({command, args});
		}

		return commandList;
	}
};

#endif