#include <iostream>
#include <memory>

//  ================================================================================================

class Visitor
{
public:

    virtual ~Visitor() = default; 

//  ----------------------------------------------------------

    virtual void visit(const class Client * client) const = 0;
    virtual void visit(const class Server * server) const = 0;
};

//  ================================================================================================

class Entity
{
public:

    virtual ~Entity() = default; 

//  ---------------------------------------------------------

    virtual void test() const = 0; 

    virtual void visit_by(const Visitor & visitor) const = 0;
};

//  ================================================================================================

class Client : public Entity 
{
public:

    void test() const override 
    { 
        std::clog << "Client::test\n"; 
    }

    void visit_by(const Visitor & visitor) const override
    { 
        visitor.visit(this);
    }
};

//  ================================================================================================

class Server : public Entity
{
public:

    void test() const override 
    { 
        std::clog << "Server::test\n"; 
    }

    void visit_by(const Visitor & visitor) const override
    {
        visitor.visit(this);
    }
};

//  ================================================================================================

class Router : public Visitor
{
public:

    void visit(const Client * client) const override 
    { 
        std::clog << "Router::visit (1)\n"; 

        client->test();
    }

    void visit(const Server * server) const override 
    {
        std::clog << "Router::visit (2)\n"; 

        server->test();
    }
};

//  ================================================================================================

int main()
{
    std::shared_ptr < Entity > entity_1 = std::make_shared < Client > ();
    std::shared_ptr < Entity > entity_2 = std::make_shared < Server > ();

    Router router;

    entity_1->visit_by(router);
    entity_2->visit_by(router);
}