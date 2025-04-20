#include "Drawer.hpp"

std::vector<size_t> Drawer::getColumnWidths(const std::vector<std::vector<std::string>>& table) {
    std::vector<size_t> colWidths;
    for (const auto& row : table) {
        if (colWidths.size() < row.size()) colWidths.resize(row.size(), 0);
    }

    for (const auto& row : table) {
        for (size_t i = 0; i < row.size(); ++i) {
        colWidths[i] = std::max(colWidths[i], row[i].length());
        }
    }
    return colWidths;
}

// Vẽ dòng kẻ ngang (dùng '+', '-')
void Drawer::printSeparator(const std::vector<size_t>& colWidths) {
    for (auto width : colWidths) {
        out << "+" << std::string(width + 2, '-'); // +2 để có khoảng trắng padding
    }
    out << "+\n";
}

// Vẽ một dòng dữ liệu
void Drawer::printRow(const std::vector<std::string>& row, const std::vector<size_t>& colWidths) {
    for (size_t i = 0; i < colWidths.size(); ++i) {
        out << "| " << std::setw(colWidths[i]) << std::left;
        if (i < row.size()) out << row[i];
        else out << ""; // nếu thiếu ô
        out << " ";
    }
    out << "|\n";
}

void Drawer::drawTable(const std::vector<std::vector<std::string>>& table, std::string fileName) {

    out = std::ofstream(fileName);

    if (!out) {
        std::cerr << "Không thể mở file để ghi.\n";
        return;
    }

    auto colWidths = getColumnWidths(table);
    printSeparator(colWidths);
    for (const auto& row : table) {
        printRow(row, colWidths);
        printSeparator(colWidths);
    }

    out.close();
    std::cerr << "Đã ghi bảng ra file: " << fileName << "\n";
}

void Drawer::write_dot(std::shared_ptr<Node> node, std::ofstream& out, int parent_id) {
    int current_id = node_id++;
    out << "  node" << current_id << " [label=\"" << node->getLabel() << "\"];\n";
    if (parent_id != -1) {
        out << "  node" << parent_id << " -> node" << current_id << ";\n";
    }
    for (auto child : node->getChildrens()) {
        write_dot(child, out, current_id);
    }
}

void Drawer::drawTree(ParseTree tree, std::string name) {
    std::ofstream out(name + ".dot");
    out << "digraph G {\n";
    write_dot(tree.getRoot(), out, -1);
    out << "}\n";
    out.close();

    std::string command = "dot -Tpng " + name + ".dot -o " + name + ".png";
    int result = system(command.c_str());

    if (result == 0) {
        std::cerr << "✅ Đã tạo thành công\n";
    } else {
        std::cerr << "❌ Lỗi khi chạy lệnh dot. Kiểm tra xem bạn đã cài Graphviz chưa.\n";
    }
}

