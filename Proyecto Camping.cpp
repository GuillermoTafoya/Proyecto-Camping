#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <string>
#include <algorithm>

bool is_empty(std::ifstream& pFile);
void flush();

//Borra la terminal
void flush() {
    system("CLS");
}

//Clase que modela un producto
class producto {
    public:
        int stock, id;
        std::string name, caducidad;

        //El constructor que le define los atributos del objeto.
        producto(std::string NAME = "", std::string CADUCIDAD = "", int STOCK = 0, int ID = 0) : name(NAME), stock(STOCK), caducidad(CADUCIDAD), id(ID){}

        void describe() { //Muestra los atributos del objeto
            std::cout << std::endl << std::setw(30) << std::left << id << std::setw(30) << name << std::setw(30) << stock << caducidad << std::endl;
        }
        
        bool operator==(const producto & obj)
        {
            return (stock == obj.stock) && (name == obj.name) && (caducidad == obj.caducidad) && (id == obj.id);
        }
        
        friend std::ostream& operator << (std::ostream& out, const producto& obj) //Define una acción a seguir cuando se utilize
        {                                                                         //un operador ostream en el objeto.
            out << std::endl << std::setw(30) << std::left << obj.id << std::setw(30) << obj.name << std::setw(30) << obj.stock << obj.caducidad;
            return out;
        }
       
        friend std::istream& operator >> (std::istream& in, producto& obj) //Define una acción a seguir cuando se utilize
        {                                                                  //un operador istream en el objeto.
            in >> obj.id;
            in >> obj.name;
            in >> obj.stock;
            in >> obj.caducidad;
            return in;
        }
};

std::vector<producto> load();

void describe(std::vector<producto> inventario);

//Guarda el vector de productos en el archivo
void save(std::vector<producto> &inventario) {
    std::ofstream file_obj;
    file_obj.open("inventario.txt", std::ios::trunc);
    std::cout << "Guardando....\n";


    file_obj << std::setw(30) << std::left << "ID" << std::setw(30) << "Producto" << std::setw(30) << "Stock" << "Caducidad";

    for (int i = 0; i < inventario.size(); i++) {
        inventario[i].id = i;
        file_obj << inventario[i];
    }

    

    std::cout << "Proceso terminado.\n";
    std::cout << std::endl << "Presione ENTER para continuar...";
    std::cin.get();
}

//Añade productos al archivo.
void add(std::vector<producto> &inventario) {
    flush();
    std::string name, caducidad, centinela;
    int stock;
    std::ifstream file;
    file.open("inventario.txt", std::ios::app);
    
    //Obtiene la llave primaria
    int id = -1;
    std::string line;
    while (std::getline(file, line))
        id++;
    file.close();

    while (true) {

        std::cout << "Para dejar de ingresar productos, escriba '-1' en cualquier momento.\n";
        std::cout << "Ingrese el nombre del producto (no use espacios): ";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //Valida el cin del nombre
        std::getline(std::cin, name);
        while (name.find(' ') != std::string::npos){
        
            std::cout << "Ingrese el nombre del producto (no use espacios): ";
            std::getline(std::cin, name);
        }
        if (name == "-1") {
            break;
        }
        std::cout << "Ingrese el stock del producto: ";
        std::cin >> stock;
        if (stock == -1){
            break;
        }
        std::cout << "Ingrese la fecha de caducidad del producto (no use espacios): ";
        std::cin >> caducidad;
        if (caducidad == "-1") {
            break;
        flush();
        }
        flush();
        producto nuevo(name, caducidad, stock, id);

        id++;
        inventario.push_back(nuevo);
    }

}

//Regresa el indice donde se encuentra un producto
auto search_by_name(std::vector<producto> &inventario, std::string name) {

    auto it = find_if(inventario.begin(), inventario.end(), [&name](const producto& obj) {return obj.name == name; });
    

        if (it != inventario.end())
        {
            auto index = std::distance(inventario.begin(), it);
            return index;
        }
        return INT_MAX;
}

//Elimina elementos del vector que contiene los productos y posteriormente guarda la versión modificada al archivo.
void obliterate(std::vector<producto> &inventario) {

    if (inventario.empty()) {
        flush();
        std::cout << "El inventario está vacio.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << std::endl << "Presione ENTER para continuar...";
        std::cin.get();
        return;
    }

    int action;
    std::string name;
    int i;
    do {
        flush();
        
        std::cout << "Elija una de las siguientes opciones:\n";
        std::cout << "1: Borrar buscando por nombre\n";
        std::cout << "2: Borrar buscando por id\n";
        std::cout << "3: Salir\n\n";
        std::cin >> action;
        std::cin.clear();

        

        switch (action) {
        default:
            std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Presione ENTER para continuar...";
            std::cin.get();
            break;
        case 1:
            
            describe(inventario);

            std::cout << "\nIngresa el producto que desea eliminar: ";
            std::cin >> name;
            i = search_by_name(inventario, name);
            if (i != INT_MAX) {
                
                inventario.erase(inventario.begin()+i);
                std::cout << "El producto <" << name << " > ha sido eliminado.\n";
                save(inventario);
                std::cin.get();
            }
            else {
                std::cout << "No existe un producto con el nombre <" << name << "> por el momento.";
                std::cout << std::endl << "Presione ENTER para continuar...";
                std::cin.get();
                std::cin.get();
            }
            
            break;
        case 2:
            describe(inventario);
            std::cout << "\nIngresa el id del producto que desea eliminar: ";
            
            std::cin >> i;

            if (i<0 || i>inventario.size() - 1) {
                std::cout << "ID invalido.";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Presione ENTER para continuar...";
                std::cin.get();
                break;
            }

            name = inventario[i].name;
            inventario.erase(inventario.begin() + i);
            std::cout << "El producto <" << name << "> ha sido eliminado.\n";
            save(inventario);
            std::cin.get();
            break;
        case 3:
            flush();
            break;
        }



    } while (action != 3);
    save(inventario);
    std::cin.get();
}

//Modifica elementos del vector que contiene los productos y posteriormente guarda la versión modificada al archivo.
void change(std::vector<producto> &inventario) {


    if (inventario.empty()) {
        flush();
        std::cout << "El inventario está vacio.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << std::endl << "Presione ENTER para continuar...";
        std::cin.get();

        return;
    }



    int action, action2, newID;
    std::string name, newAttribute;
    int i;
    do {
        flush();
        std::cout << "Elija una de las siguientes opciones:\n";
        std::cout << "1: Editar buscando por nombre\n";
        std::cout << "2: Editar buscando por id\n";
        std::cout << "3: Salir\n\n";
        std::cin >> action;
        action2 = 0;
        std::cin.clear();

        

        switch (action) {
        default:
            std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Presione ENTER para continuar...";
            std::cin.get();
            break;
        case 1:

            describe(inventario);

            std::cout << "\nIngresa el producto que desea editar: ";
            std::cin >> name;
            i = search_by_name(inventario, name);
            if (i != INT_MAX) {


                while (action2 != 4) {

                    flush();
                    std::cout << "Que busca editar del producto <" << name << "> ?\n";
                    std::cout << "1: Editar nombre\n";
                    std::cout << "2: Editar stock\n";
                    std::cout << "3: Editar caducidad\n";
                    std::cout << "4: Regresar\n";

                    std::cin >> action2;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    switch (action2) {
                    default:
                        std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << std::endl << "Presione ENTER para continuar...";
                        std::cin.get();
                        break;
                    case 1:

                        std::cout << "Ingrese el nuevo nombre del producto (no use espacios): ";
                        std::getline(std::cin, newAttribute);
                        while (newAttribute.find(' ') != std::string::npos) {

                            std::cout << "Ingrese el nuevo nombre del producto (no use espacios): ";
                            std::getline(std::cin, newAttribute);
                        }

                        inventario[i].name = newAttribute;
                        save(inventario);
                        std::cin.get();

                        break;
                    case 2:


                        std::cout << "Ingrese el nuevo stock: ";
                        std::cin >> newID;

                        inventario[i].stock = newID;
                        save(inventario);
                        std::cin.get();

                        break;
                    case 3:
                        std::cout << "Ingrese la fecha de caducidad del producto (no use espacios): ";
                        std::cin >> newAttribute;

                        inventario[i].caducidad = newAttribute;
                        save(inventario);
                        std::cin.get();
                        break;
                    case 4:
                        break;
                    }
                }


            }
            else {
                std::cout << "No existe un producto con el nombre <" << name << "> por el momento.";
                std::cout << std::endl << "Presione ENTER para continuar...";
                std::cin.get();
                std::cin.get();
                break;
            }

            break;
        case 2:

            describe(inventario);

            std::cout << "\nIngresa el id del producto que desea modificar: ";
            std::cin >> i;

            if (i<0 || i>inventario.size() - 1) {
                std::cout << "ID invalido.";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << "Presione ENTER para continuar...";
                std::cin.get();
                break;
            }

            while (action2 != 4) {

                flush();
                std::cout << "Que busca editar del producto <" << inventario[i].name << "> ?\n";
                std::cout << "1: Editar nombre\n";
                std::cout << "2: Editar stock\n";
                std::cout << "3: Editar caducidad\n";
                std::cout << "4: Regresar\n";

                std::cin >> action2;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                switch (action2) {
                default:
                    std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << std::endl << "Presione ENTER para continuar...";
                    std::cin.get();
                    break;
                case 1:

                    std::cout << "Ingrese el nuevo nombre del producto (no use espacios): ";
                    std::getline(std::cin, newAttribute);
                    while (newAttribute.find(' ') != std::string::npos) {

                        std::cout << "Ingrese el nuevo nombre del producto (no use espacios): ";
                        std::getline(std::cin, newAttribute);
                    }

                    inventario[i].name = newAttribute;
                    save(inventario);

                    break;
                case 2:

                    std::cout << "Ingrese el nuevo stock: ";
                    std::cin >> newID;

                    inventario[i].stock = newID;
                    save(inventario);
                    std::cin.get();

                    break;
                case 3:
                    std::cout << "Ingrese la fecha de caducidad del producto (no use espacios): ";
                    std::cin >> newAttribute;

                    inventario[i].caducidad = newAttribute;
                    save(inventario);
                    std::cin.get();
                    break;
                case 4:
                    break;
                }
            }

        break;



        case 3: {
            flush();
            break;
            }
        }
           
    } while (action != 3);

    save(inventario);
    std::cin.get();
}

//Crea un vector de los productos para guardar temporalmente la información en la RAM y facilitar su manipulación.
std::vector<producto> load() {
    std::vector<producto> inventario;
    std::string helper;
    std::ifstream file_obj("inventario.txt");
    producto obj;

    std::getline(file_obj, helper);

    while (!file_obj.eof()) {
        assert(file_obj >> obj);
        inventario.push_back(obj);
    }

    file_obj.close();

    return inventario;
}

//Muestra los objetos como una tabla
void describe(std::vector<producto> inventario) {

    flush();

    if (inventario.empty()) {
        std::cout << "No hay nada en el inventario.\n";
    }
    else {
        std::cout << std::endl << std::setw(30) << std::left << "ID" << std::setw(30) << "Producto" << std::setw(30) << "Stock" << "Caducidad" << std::endl;
    }

    for (int i = 0; i < inventario.size(); i++) {
        inventario[i].describe();
    }
    
}

//Comprueba si el archivo está vacio
bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main()
{
    int action;
    std::ifstream file_obj("inventario.txt");
    if (is_empty(file_obj)){
       file_obj.close();
       std::ofstream file_obj("inventario.txt");
       file_obj << std::setw(30) << std::left << "ID" << std::setw(30) << "Producto" << std::setw(30) << "Stock" << "Caducidad";
    }
    file_obj.close();
    std::vector<producto> inventario = load();
    do {
        flush();
        std::cout << "Elija una de las siguientes opciones:\n"; 
        std::cout << "1: Dar de alta\n";
        std::cout << "2: Dar de baja\n";
        std::cout << "3: Hacer un cambio\n";
        std::cout << "4: Ver resumen\n";
        std::cout << "5: Salir\n";
        std::cin >> action;
        std::cin.clear();

        switch (action) {
        default:
            std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Presione ENTER para continuar...";
            std::cin.get();
            break;
        case -1:
            inventario = load();
            break;
        case 1:
            add(inventario);
            save(inventario);
            break;
        case 2:
            obliterate(inventario);
            
            break;
        case 3:
            change(inventario);
            
            break;
        case 4:
            describe(inventario);
            std::cout << std::endl << "Presione ENTER para continuar...";
            std::cin.get();
            std::cin.get();
            break;
        case 5:
            save(inventario);
            break;
        }

    } while (action != 5);

    return 0;
}
