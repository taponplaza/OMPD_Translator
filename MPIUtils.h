#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <sstream>
#include <fstream>
#include <vector>
#include "SymbolTable.h"

extern std::ofstream generatedFile;

class Funcion
{

private:
    std::ostringstream variables, ini_seq_pre, c_seq_pre, end_seq_pre, pragmas, ini_seq_pos, c_seq_pos, end_seq_pos, ret;

public:
    void write_MPI_sec_pre()
    {
        if (ini_seq_pre.str().empty() && end_seq_pre.str().empty())
        {
            ini_seq_pre << "\nif (__taskid == 0) {\n";
            end_seq_pre << "}\n";
        }
    }

    void write_MPI_seq_pos()
    {
        if (ini_seq_pos.str().empty() && end_seq_pos.str().empty())
        {
            ini_seq_pos << "\nif (__taskid == 0) {\n";
            end_seq_pos << "}\n";
        }
    }

    void insert_MPI_Funcion(std::string line, int state)
    {
        switch (state)
        {
        case 1:
            variables << line;
            break;
        case 2:
            c_seq_pre << line;
            break;
        case 3:
            write_MPI_sec_pre();
            pragmas << line;
            write_MPI_seq_pos();
            break;
        case 4:
            c_seq_pos << line;
            break;
        case 5:
            ret << line;
            break;
        }
    }

    std::string print()
    {
        return variables.str() + ini_seq_pre.str() + c_seq_pre.str() + end_seq_pre.str() + pragmas.str() + ini_seq_pos.str() + c_seq_pos.str() + end_seq_pos.str() + ret.str();
    }

    ~Funcion()
    {
        // Release any resources that were allocated in this object
    }
};

class MainFuncion
{

private:
    std::ostringstream variables, inic, inic_type_def, ini_seq_pre, c_seq_pre, end_seq_pre, pragmas, ini_seq_pos, c_seq_pos, end_seq_pos, c_fi, ret;

public:
    MainFuncion()
    {
        inic << "MPI_Init(&argc, &argv);\n";
        inic << "MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);\n";
        inic << "MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);\n\n";

        c_fi << "MPI_Finalize();\n";
    }

    void write_type_def()
    {
        if (inic_type_def.str().empty())
            inic_type_def << "\nDeclare_MPI_Types();\n";
    }

    void write_MPI_sec_pre()
    {
        if (ini_seq_pre.str().empty() && end_seq_pre.str().empty())
        {
            ini_seq_pre << "\nif (__taskid == 0) {\n";
            end_seq_pre << "}\n";
        }
    }

    void write_MPI_seq_pos()
    {
        if (ini_seq_pos.str().empty() && end_seq_pos.str().empty())
        {
            ini_seq_pos << "\nif (__taskid == 0) {\n";
            end_seq_pos << "}\n";
        }
    }

    void insert_MPI_Main(std::string line, int state)
    {
        switch (state)
        {
        case 1:
            variables << line;
            break;
        case 2:
            c_seq_pre << line;
            break;
        case 3:
            write_MPI_sec_pre();
            pragmas << line;
            write_MPI_seq_pos();
            break;
        case 4:
            c_seq_pos << line;
            break;
        case 5:
            ret << line;
            break;
        }
    }

    std::string print()
    {
        return variables.str() + inic.str() + inic_type_def.str() + ini_seq_pre.str() + c_seq_pre.str() + end_seq_pre.str() + pragmas.str() + ini_seq_pos.str() + c_seq_pos.str() + end_seq_pos.str() + c_fi.str() + ret.str();
    }

    ~MainFuncion()
    {
        // Release any resources that were allocated in this object
    }
};

class MPIUtils
{

private:
    std::ostringstream includes, var_glob;
    std::vector<Funcion *> *funciones = new std::vector<Funcion *>();
    MainFuncion *mainFuncion = new MainFuncion();

    std::ostringstream line;

public:
    void write_MPI_Type_struct(SymbolInfo *symbol)
    {
        insert_MPI_buffer_line(0, 1);
        var_glob << "\n\nMPI_Datatype MPI" << symbol->getSymbolName() << "_t;\n\n";
        var_glob << "void __Declare_MPI_Type_" << symbol->getSymbolName() << "() {\n";
        var_glob << "    int blocklengths[" << symbol->getParamList()->size() << "];\n";
        var_glob << "    MPI_Datatype old_types[" << symbol->getParamList()->size() << "];\n";
        var_glob << "    MPI_Aint disp[" << symbol->getParamList()->size() << "];\n";
        var_glob << "    MPI_Aint lb;\n";
        var_glob << "    MPI_Aint extent;\n";
        for (std::vector<SymbolInfo *>::size_type i = 0; i < symbol->getParamList()->size(); i++)
        {
            var_glob << "    blocklengths[" << i << "] = 1;\n";
        }
        for (std::vector<SymbolInfo *>::size_type i = 0; i < symbol->getParamList()->size(); i++)
        {
            var_glob << "    old_types[" << i << "] = MPI_" << symbol->getParamList()->at(i)->getVariableType() << ";\n";
        }
        for (std::vector<SymbolInfo *>::size_type i = 0; i < symbol->getParamList()->size(); i++)
        {
            var_glob << "    MPI_Type_get_extent(MPI_" << symbol->getParamList()->at(i)->getVariableType() << ", &lb, &extent);\n";
            if (i == 0)
                var_glob << "    disp[" << i << "] = lb;\n";
            else
                var_glob << "    disp[" << i << "] = disp[" << i - 1 << "] + extent;\n";
        }
        var_glob << "    MPI_Type_create_struct(" << symbol->getParamList()->size() << ", blocklengths, disp, old_types, &MPI" << symbol->getSymbolName() << "_t);\n";
        var_glob << "    MPI_Type_commit(&MPI" << symbol->getSymbolName() << "_t);\n";
        var_glob << "}\n\n";
        var_glob << "void Declare_MPI_Types() {\n";
        var_glob << "    __Declare_MPI_Type_" << symbol->getSymbolName() << "();\n";
        var_glob << "    return;\n";
        var_glob << "}\n";

        mainFuncion->write_type_def();
    }

    void write_MPI_header()
    {
        includes << "#include <assert.h>\n";
        includes << "#include <mpi.h>\n";
        var_glob << "\nint __taskid = -1, __numprocs = -1;\n\n";
    }

    void write_MPI_new_func()
    {
        Funcion *newFuncion = new Funcion();
        funciones->push_back(newFuncion);
    }

    void insert_MPI_token(std::string token, int level, int state)
    {
        if (token == "\n")
        {
            line << token;
            insert_MPI(line.str(), level, state);
            line = std::ostringstream();
        }
        else
        {
            line << token;
        }
    }

    void insert_MPI_buffer_line(int level, int state)
    {
        insert_MPI(line.str(), level, state);
        line = std::ostringstream();
    }

    void insert_MPI(std::string line, int level, int state)
    {
        switch (level)
        {
        case 0:
            insert_MPI_Global(line, state);
            break;
        case 1:
            if (!funciones->empty())
            {
                Funcion *lastFuncion = funciones->back();
                lastFuncion->insert_MPI_Funcion(line, state);
            }
            break;
        case 2:
            mainFuncion->insert_MPI_Main(line, state);
            break;
        }
    }

    void insert_MPI_Global(std::string line, int state)
    {
        switch (state)
        {
        case 0:
            includes << line;
            break;
        case 1:
            var_glob << line;
            break;
        }
    }

    void generate_MPI_all()
    {
        generatedFile << includes.str() << var_glob.str();
        for (std::vector<Funcion *>::size_type i = 0; i < funciones->size(); i++)
        {
            generatedFile << funciones->at(i)->print();
        }
        generatedFile << mainFuncion->print();
    }

    ~MPIUtils()
    {
        delete mainFuncion;
        for (auto funcion : *funciones)
        {
            delete funcion;
        }
        delete funciones;
    }
};

#endif // MPI_UTILS_H
