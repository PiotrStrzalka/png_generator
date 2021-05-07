#include <csignal>
#include <iostream>
#include <pngwriter.h>
#include <cli/cli.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class Shape
{
    public:
        virtual void draw(pngwriter &writer) = 0;
        virtual ~Shape(){};
};

class Rectangle :  public Shape
{
    int x, y, w, h;
    public:
        Rectangle(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h} {};

        void draw(pngwriter &writer)
        {
            writer.filledsquare(this->x, this->y, (this->x+this->w), (this->y+this->h), 128, 128, 128);
        }
};

class Triangle : public Shape
{
    int x1, y1, x2, y2, x3, y3;
    public:
        Triangle(int x1, int y1, int x2, int y2, int x3, int y3)
                    : x1{x1}, y1{y1}, x2{x2}, y2{y2}, x3{x3}, y3{y3} {};
    
        void draw(pngwriter &writer) override
        {
            writer.filledtriangle(x1, y1, x2, y2, x3, y3, 128, 128, 128);
        }
};

class Generator 
{
    private:
        int height = 200;
        int width = 200;
        std::string name = "default";
        std::vector<std::shared_ptr<Shape>> objects;

        void setDefaultValues(void)
        {
            objects.clear();
            height = 200;
            width = 200;
            name = "default";
        }
        Generator(){};

    public:
        Generator(Generator &other) = delete;
        void operator=(const Generator &) = delete;
        static Generator& getInstance()
        {
            static Generator generator;
            return generator;
        }

        void addObject(std::shared_ptr<Shape> obj)
        {
            objects.push_back(obj);
        }

        void setWidth(int w) {width = w;}
        void setHeight(int h) {height = h;}
        void setName(std::string s){name = s;}
        void render(void)
        {
            pngwriter writer(width, height, 0xFFFFFF, name.c_str());
            for(auto &obj : objects)
            {
                obj->draw(writer);
            }
            writer.close();
            setDefaultValues();
        }

        std::string toString(void)
        {
            return "Blublublu: " + objects.size();
        }
};

/* REQ4 */
void prepareRootMenu(std::unique_ptr<cli::Menu> &menu)
{
    menu->Insert("exit", [](std::ostream& out){std::exit(0);},
                "Bye Bye");    
    /* REQ9 */
    menu->Insert("set_width",[](std::ostream& out, int x){
            //todo validation required
            Generator::getInstance().setWidth(x);
            out << "Screen width set to: " << x  << " pixels\n";
            },
        "Sets screen width");

    /* REQ10 */
    menu->Insert("set_height",[](std::ostream& out, int x){
            //todo validation required
            Generator::getInstance().setHeight(x);
            out << "Screen height set to: " << x  << " pixels\n";},
        "Sets screen height");

    /* REQ11 */
    menu->Insert("draw_rectangle",[](std::ostream& out, int x, int y, int w, int h)
        {
            auto r = std::make_shared<Rectangle>(x,y,w,h);
            Generator::getInstance().addObject(r);
            out << "Rectangle params saved\n";
        },            
        "Set rectangle as object to draw [ e.g. 'draw_rectangle 10 10 20 30' ]");
    
    /* REQ12 */
    menu->Insert("draw_triangle",[](std::ostream& out, int x1, int y1, int x2, int y2, int x3, int y3)
        {
            auto t = std::make_shared<Triangle>(x1,y1,x2,y2,x3,y3);
            Generator::getInstance().addObject(t);            
            out << "Triangle params saved\n";
        },            
        "Set triangle as object to draw [ e.g. 'draw_triangle 1 2 3 4 5 6' ]");
    
    /* REQ13 */
    menu->Insert("exception",[](std::ostream& out)
        {
            out << "Exception generated" << std::endl;
            throw std::invalid_argument("Exception text");
        },
        "Generate exception\n"
    );

    menu->Insert("render",[](std::ostream& out, std::string choosenName)
        {
            Generator::getInstance().setName(choosenName);
            Generator::getInstance().render();
            auto path = std::string(fs::current_path().append(choosenName));
            out << "Drawing finished, element can be found on path: "
                << path << """" << std::endl;
        },
        "Draw object with specific name"
    );
}


class CliTerminalSession : public cli::CliSession
{
    public:
        CliTerminalSession(cli::Cli& _cli, std::istream& _in = std::cin, std::ostream& _out = std::cout) :
            CliSession(_cli, _out, 100),
            exitFlag(false),
            in(_in)
        {
            if(!_in.good()){ throw std::invalid_argument("istream_invalid argument");}
            if(!_out.good()){throw std::invalid_argument("outstream_invalid argument");}
            ExitAction(
                [this](std::ostream&){
                    exitFlag = true;
                }
            );
        }
        
        void Start()
        {
            while(!exitFlag)
            {
                Prompt();
                std::string line;
                if (!in.good())
                    Exit();
                std::getline(in, line);
                if (in.eof())
                    Exit();
                else
                    if(line.size())
                    {
                        Feed(line);
                    }                
            }
        }
    private:
        std::istream& in;
    bool exitFlag;
};

int main(int argc, char** argv)
{
    /* REQ4 */
    auto rootMenu = std::make_unique<cli::Menu>("");
    prepareRootMenu(rootMenu);

    cli::Cli cli (std::move(rootMenu));
    cli.StdExceptionHandler(
        /* REQ7 */
        [](std::ostream& out, const std::string& cmd, const std::exception& e)
            {
                std::cerr << "Exception caught in cli handler: "
                    << e.what()
                    << " handling command: "
                    << cmd
                    << ".\n";
            }
    );
    cli.ExitAction([](auto& out){out << "Bye, thanks for using!" << std::endl;});
    CliTerminalSession input(cli);

    

    input.Start();    
    return 0;
}