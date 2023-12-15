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

    IF_STATEMENT_NODE,
    ELSE_IF_STATEMENT_NODE,
    ASSIGMENT_NODE, // ?

    VARIABLE_NODE,
    
    PREPROCESSOR_DIRECTIVE_NODE,
    DERECTIVE_INCLUDE_NODE,
    COMPOUND_STATEMENTS_NODE,
    INT_VARIABLE_NODE,
    DOUBLE_VARIABLE_NODE,

    

    WHILE_LOOP_NODE,
    DO_WHILE_LOOP_NODE,
    FOR_LOOP_NODE,

    BREAK_STATEMENT_NODE,
    CONTINUE_STATEMENT_NODE,


    SWITHC_STATEMENT_NODE,
    CASE_NODE,
    DEFAULT_NODE,

    ELSE_STATEMENT_NODE,

    FUNCTION_DECLARATION_NODE,
    
    PARAMETER_NODE,
    RETURN_STATEMENT_NODE,

    BINARY_OPERATION_NODE,
    UNARY_OPERATION_NODE,
    
    TERNARY_CONDITIONAL_NODE,

    LINE_COMMENT_NODE,
    BLOCK_COMMENT_NODE,

    NEW_EXPRESSION_NODE,
    DELETE_EXPRESSION_NODE,

    ACCESS_OPERATOR_NODE,

    UNDEFINED_NODE
};

class ASTNode {
public:
    explicit ASTNode(ASTNodeType type) : type(type) {}

    ASTNodeType getType() const {
        return type;
    }

    virtual void print(int indent = 0) const = 0;

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

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ProgramNode:" << std::endl;
        for (const auto& statement : statements) {
            statement->print(indent + 2);
        }
    }

private:
    std::vector<ASTNode*> statements;
};

//class PreprocessorDirectiveNode : public ASTNode {
//public:
//    PreprocessorDirectiveNode(const std::vector<ASTNode*>& statements) : ASTNode(ASTNodeType::PREPROCESSOR_DIRECTIVE_NODE),
//        statements(statements) {};
//private:
//    std::vector<ASTNode*> statements;
//};

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
    FunctionDeclarationNode(ASTNode* parameterListNode, ASTNode* compoundStatementNode, std::string name, token_type returnType) : ASTNode(ASTNodeType::FUNCTION_DECLARATION_NODE),
        parameterListNode(parameterListNode), compoundStatementNode(compoundStatementNode), name(name), returnType(returnType) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "FunctionDeclarationNode: " << name << std::endl;
        parameterListNode->print(indent + 2);
        compoundStatementNode->print(indent + 2);
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
            statement->print(indent + 2);
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
            statement->print(indent + 2);
        }
    }
private:
    std::vector<ASTNode*> statements;
};

class ParameterNode : public ASTNode {
public:
    ParameterNode(std::string name, std::string type) : ASTNode(ASTNodeType::PARAMETER_NODE), name(name), type(type) {};
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ParameterNode: " << type << " " << name << std::endl;
    }
private:
    std::string name;
    std::string type;
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
    BinaryOperationNode(ASTNode* leftOperand, token_type op, ASTNode* rightOperand) : ASTNode(ASTNodeType::BINARY_OPERATION_NODE),
        leftOperand(leftOperand), op(op), rightOperand(rightOperand) {};

    ASTNode* getLeft() const {
        return leftOperand;
    }

    token_type getOperator() const {
        return op;
    }

    ASTNode* getRight() const {
        return rightOperand;
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BinaryOperationNode: " << token_type_to_string(op) << std::endl;
        leftOperand->print(indent + 2);
        rightOperand->print(indent + 2);
    }
private:
    std::string token_type_to_string(token_type type) const {
        switch (type)
        {
        case token_type::GREATER:
        {
            return "greater";
        }
        case token_type::LESS_EQUAL:
        {
            return "less and equal";
        }
        case token_type::GREATER_EQUAL:
        {
            return "greater and equal";
        }
        case token_type::EQUAL:
        {
            return "equal";
        }
        case token_type::NOT_EQUAL:
        {
            return "not equal";
        }
        case token_type::AND:
        {
            return "logic and";
        }
        case token_type::OR:
        {
            return "logic or";
        }
        case token_type::EXCLAMATION:
        {
            return "exclamation";
        }
        case token_type::PLUS:
        {
            return "plus";
        }
        case token_type::MINUS:
        {
            return "minus";
        }
        case token_type::STAR:
        {
            return "star";
        }
        case token_type::SLASH:
        {
            return "true";
        }
        case token_type::INC:
        {
            return "inc";
        }
        case token_type::DEC:
        {
            return "dec";
        }
        case token_type::ASSIGN:
        {
            return "assign";
        }
        case token_type::ADD_ASSIGN:
        {
            return "add assign";
        }
        case token_type::SUB_ASSIGN:
        {
            return "sun assign";
        }
        case token_type::MUL_ASSIGN:
        {
            return "mul assign";
        }
        case token_type::DIV_ASSIGN:
        {
            return "true";
        }
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
            value->print(indent + 2);
        }
    }
private:
    std::string name;
    ASTNode* value;
};

class IntVariableNode : public ASTNode {
public:
    IntVariableNode(std::string value) : ASTNode(ASTNodeType::INT_VARIABLE_NODE),
        value(std::stoi(value)) {};
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "IntVariableNode: " << value << std::endl;
    }
private:
    int value;
};

class DoubleVariableNode : public ASTNode {
public:
    DoubleVariableNode(std::string name, std::string value) : ASTNode(ASTNodeType::DOUBLE_VARIABLE_NODE),
        name(name), value(std::stod(value)) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "DoubleVariableNode: " << name << " = " << value << std::endl;
    }
private:
    std::string name;
    double value;
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
        condition->print(indent + 2);
        statements->print(indent + 2);
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
        condition->print(indent + 2);
        statements->print(indent + 2);
        if (elseNode != nullptr) {
            elseNode->print(indent + 2);
        }
    }
private:
    ASTNode* condition;
    ASTNode* statements;
    ASTNode* elseNode;
};

class ElseNode : public ASTNode {
public:
    ElseNode(ASTNode* statements) : ASTNode(ASTNodeType::ELSE_STATEMENT_NODE),
        statements(statements) {};
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ElseNode:" << std::endl;
        statements->print(indent + 2);
    }
private:
    ASTNode* statements;
};
class AssignmentStatementNode : public ASTNode {
public:

    AssignmentStatementNode(IdentifierNode* variable, ASTNode* value) : ASTNode(ASTNodeType::ASSIGMENT_NODE),
        variable(variable), value(value) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "AssignmentStatementNode:" << std::endl;
        variable->print(indent + 2);
        value->print(indent + 2);
    }
private:
    IdentifierNode* variable;
    ASTNode* value;
};

