#pragma once
#include <iostream>
#include <string>
#include <vector>


enum class ASTNodeType {
    UNKNOWN_NODE,

    PROGRAM_NODE,
    COMPOUND_STATEMENT_NODE,
    VARIABLE_NODE,
    IDENTIFIER_NODE,
    DERECTIVE_INCLUDE_NODE,
    LITERAL_NODE,

    VARIABLE_DECLARATION_NODE,

    WHILE_LOOP_NODE,
    DO_WHILE_LOOP_NODE,
    FOR_LOOP_NODE,

    BREAK_STATEMENT_NODE,
    CONTINUE_STATEMENT_NODE,


    SWITHC_STATEMENT_NODE,
    CASE_NODE,
    DEFAULT_NODE,

    IF_STATEMENT_NODE,
    ELSE_STATEMENT_NODE,

    FUNCTION_DECLARATION_NODE,
    PARAMETER_LIST_NODE,
    PARAMETER_NODE,
    RETURN_STATEMENT_NODE,

    BINARY_OPERATION_NODE,
    UNARY_OPERATION_NODE,
    ASSIGMENT_NODE,
    TERNARY_CONDITIONAL_NODE,

    LINE_COMMENT_NODE,
    BLOCK_COMMENT_NODE,

    NEW_EXPRESSION_NODE,
    DELETE_EXPRESSION_NODE,

    PREPROCESSOR_DIRECTIVE_NODE,

    ACCESS_OPERATOR_NODE,

    UNDEFINED_NODE
};

class ASTNode {
public:
    explicit ASTNode(ASTNodeType type) : type(type) {}

    ASTNodeType getType() const {
        return type;
    }

private:
    ASTNodeType type;
};

class ProgramNode : public ASTNode {
public:
    ProgramNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::PROGRAM_NODE),
        statements(statements) {}

    const std::vector<ASTNode*>& getStatements() const {
        return statements;
    }

private:
    std::vector<ASTNode*> statements;
};

class DerectivIncludeNode : public ASTNode {
public:
    DerectivIncludeNode(std::string fileName) : ASTNode(ASTNodeType::DERECTIVE_INCLUDE_NODE),
        fileName(fileName) {};
private:
    std::string fileName;
};


class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::FUNCTION_DECLARATION_NODE),
        statements(statements), name(name), returnType(returnType) {}

    const std::vector<ASTNode*>& getStatements() const {
        return statements;
    }

private:
    std::vector<ASTNode*> statements;
    std::string name;
    std::string returnType;
};

class ParameterListNode : public ASTNode {
public:
    ParameterListNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::PARAMETER_LIST_NODE),
        statements(statements) {}

private:
    std::vector<ASTNode*> statements;
};

class ParameterNode : public ASTNode {
public:
    ParameterNode(std::string name, std::string type) : ASTNode(ASTNodeType::PARAMETER_NODE), name(name), type(type) {};

private:
    std::string name;
    std::string type;
};

class CompoundStatementNode : public ASTNode {
public:
    CompoundStatementNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::COMPOUND_STATEMENT_NODE),
        statements(statements) {};
private:
    std::vector<ASTNode*> statements;

};

class ReturnStatementNode : public ASTNode {
public:
    ReturnStatementNode(ASTNode* node) : ASTNode(ASTNodeType::RETURN_STATEMENT_NODE), 
        node(node) {};

private:
    ASTNode* node;
};

class BinaryOperationNode : public ASTNode {
public:
    BinaryOperationNode(ASTNode* leftOperand, std::string op, ASTNode* rightOperand) : ASTNode(ASTNodeType::BINARY_OPERATION_NODE),
        leftOperand(leftOperand), op(op), rightOperand(rightOperand) {};

private:
    ASTNode* leftOperand;
    std::string op;
    ASTNode* rightOperand;
};

class VariableNode : public ASTNode {
public:
    VariableNode(std::string type, std::string name) : ASTNode(ASTNodeType::VARIABLE_NODE),
        type(type), name(name) {};

private:
    std::string type;
    std::string name;
};
