#pragma once
#include <iostream>
#include <string>
#include <vector>


enum class ASTNodeType {
    PROGRAM_NODE,
    PARAMETER_LIST_NODE,
    COMPOUND_STATEMENT_NODE,
    VARIABLE_DECLARATION_NODE,
    LITERAL_NODE,
    BINARY_EXPRESSION_NODE,
    IDENTIFIER_NODE,
    VARIABLE_NODE,
    IF_STATEMENT_NODE,
    ELSE_IF_STATEMENT_NODE,
    ASSIGMENT_NODE, // ?
    DERECTIVE_INCLUDE_NODE,
    COMPOUND_STATEMENTS_NODE,
    FUNCTION_DECLARATION_NODE,
    PARAMETER_NODE,
    BINARY_OPERATION_NODE,
    RETURN_STATEMENT_NODE
};

class ASTNode {
public:
    explicit ASTNode(ASTNodeType type) : type(type) {}

    virtual void print(int indent = 0) const = 0;

private:
    ASTNodeType type;
};

class ProgramNode : public ASTNode {
public:
    ProgramNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::PROGRAM_NODE),
        statements(statements) {}


    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ProgramNode:" << std::endl;
        for (const auto& statement : statements) {
            statement->print(indent + 4);
        }
    }

private:
    std::vector<ASTNode*> statements;
};


class DerectivIncludeNode : public ASTNode {
public:
    DerectivIncludeNode(std::string fileName) : ASTNode(ASTNodeType::DERECTIVE_INCLUDE_NODE),
        fileName(fileName) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "DirectiveIncludeNode: " << fileName << std::endl;
    }
private:
    std::string fileName;
};


class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(ASTNode* parameterListNode, ASTNode* compoundStatementNode,  std::string name, token_type returnType) : ASTNode(ASTNodeType::FUNCTION_DECLARATION_NODE),
        parameterListNode(parameterListNode), compoundStatementNode(compoundStatementNode), name(name), returnType(returnType) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "FunctionDeclarationNode: " << name << std::endl;
        parameterListNode->print(indent + 4);
        compoundStatementNode->print(indent + 4);
    }

private:
    ASTNode* parameterListNode;
    ASTNode* compoundStatementNode;
    std::string name;
    token_type returnType;
};

class ParameterListNode : public ASTNode {
public:
    ParameterListNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::PARAMETER_LIST_NODE),
        statements(statements) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ParameterListNode:" << std::endl;
        for (const auto& statement : statements) {
            statement->print(indent + 4);
        }
    }
private:
    std::vector<ASTNode*> statements;
};

class CompoundStatementNode : public ASTNode {
public:
    CompoundStatementNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::COMPOUND_STATEMENTS_NODE),
        statements(statements) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "CompoundStatementNode:" << std::endl;
        for (const auto& statement : statements) {
            statement->print(indent + 4);
        }
    }
private:
    std::vector<ASTNode*> statements;
};

class ReturnStatementNode : public ASTNode {
public:
    ReturnStatementNode(ASTNode* expressionNode) : ASTNode(ASTNodeType::RETURN_STATEMENT_NODE),
        expressionNode(expressionNode) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ReturnStatementNode: ";
        if (expressionNode != nullptr) {
            expressionNode->print();
        } else {
            std::cout << "null\n";
        }
    }
private:
    ASTNode* expressionNode;
};

class BinaryOperationNode : public ASTNode {
public:
    BinaryOperationNode(ASTNode* leftOperand, token_type op, ASTNode* rightOperand) : ASTNode(ASTNodeType::BINARY_OPERATION_NODE),
        leftOperand(leftOperand), op(op), rightOperand(rightOperand) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BinaryOperationNode: " << token_type_to_string(op) << std::endl;
        leftOperand->print(indent + 4);
        rightOperand->print(indent + 4);
    }
private:
    std::string token_type_to_string(token_type type) const {
    switch (type) {
        case token_type::PLUS: return "+";
        case token_type::MINUS: return "-";
        case token_type::STAR: return "*";
        case token_type::SLASH: return "/";
        case token_type::EQUAL: return "==";
        case token_type::LESS: return "<";
        case token_type::GREATER: return ">";
        case token_type::LESS_EQUAL: return "<=";
        case token_type::GREATER_EQUAL: return ">=";
        case token_type::NOT_EQUAL: return "!=";
        case token_type::AND: return "&&";
        case token_type::OR: return "||";
        case token_type::ASSIGN: return "=";
        default: return "Unknown";
    }
}

    ASTNode* leftOperand;
    token_type op;
    ASTNode* rightOperand;
};

class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(std::string name, ASTNode* value) : ASTNode(ASTNodeType::VARIABLE_NODE),
        name(name), value(value) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "VariableDeclarationNode: " << name << std::endl;
        if (value != nullptr) {
            value->print(indent + 4);
        }
    }
private:
    std::string name;
    ASTNode* value;
};

class LiteralNode : public ASTNode {
public:
    LiteralNode(std::string value) : ASTNode(ASTNodeType::LITERAL_NODE),
        value(value) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "LiteralNode: " << value << std::endl;
    }
private:
    std::string value;
};


class IdentifierNode : public ASTNode {
public:
    IdentifierNode(std::string identifier) : ASTNode(ASTNodeType::IDENTIFIER_NODE),
        identifier(identifier) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "IdentifierNode: " << identifier << std::endl;
    }
private:
    std::string identifier;
};


class IfNode : public ASTNode {
public:
    IfNode(ASTNode* condition, ASTNode* statements) : ASTNode(ASTNodeType::IF_STATEMENT_NODE),
        condition(condition), statements(statements) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "IfNode:" << std::endl;
        condition->print(indent + 4);
        statements->print(indent + 4);
    }
private:
    ASTNode* condition;
    ASTNode* statements;

};

class ElseIFNode : public ASTNode {
public:
    ElseIFNode(ASTNode* condition, ASTNode* statements, ASTNode* elseNode) : ASTNode(ASTNodeType::ELSE_IF_STATEMENT_NODE),
        condition(condition), statements(statements), elseNode(elseNode) {};

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ElseIFNode:" << std::endl;
        condition->print(indent + 4);
        statements->print(indent + 4);
        if (elseNode != nullptr) {
            elseNode->print(indent + 4);
        }
    }
private:
    ASTNode* condition;
    ASTNode* statements;
    ASTNode* elseNode;
};


class AssignmentStatementNode : public ASTNode {
public:

    AssignmentStatementNode(IdentifierNode* variable, ASTNode* value) : ASTNode(ASTNodeType::ASSIGMENT_NODE),
        variable(variable), value(value) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "AssignmentStatementNode:" << std::endl;
        variable->print(indent + 4);
        value->print(indent + 4);
    }
private:
    IdentifierNode* variable;
    ASTNode* value;
};

