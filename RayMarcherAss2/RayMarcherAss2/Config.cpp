WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
/*  The following code is a VERY heavily modified from code originally sourced from:
	Ray tracing tutorial of http://www.codermind.com/articles/Raytracer-in-C++-Introduction-What-is-ray-tracing.html
	It is free to use for educational purpose and cannot be redistributed outside of the tutorial pages. */

// YOU SHOULD _NOT_ NEED TO MODIFY THIS FILE (FOR ASSIGNMENT 1, 2, OR 3)
// 
// THIS FILE DOES NOT FOLLOW THE SAME FORMATTING CONVENTIONS AS THE REST OF THIS PROJECT

#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include "Config.h"

#pragma warning(push)
#pragma warning(disable:4996)

// This is a simple config file parser.
// It does what we need no more no less.

typedef std::map<SimpleString,SimpleString> VariableSet;
typedef std::set<SimpleString> StringSet;

static SimpleString preprocess(std::fstream& InputFile) {
    SimpleString tmp;
	std::string line;
    while (getline(InputFile,line, '\n')) {
        // Get rid of C-Style comments 
        line = line.substr(0,line.find("//"));
        // Get rid of spaces/tabs/newlines. The basic syntax allows us to do that obviously.
		for (std::string::iterator i = line.begin(); i != line.end(); ++i) {
            switch (*i) {
            case ' ':case '\t': case '\n': case '\r':
                break;
            default:
              tmp.append(*i);
            }
        }
    }
    return tmp;
}

static bool preload(std::fstream& InputFile, VariableSet &variables, StringSet &sections) {
    static SimpleString buf;
    SimpleString tmp;
    int recursion=0;
    tmp = preprocess(InputFile);
    SimpleString::iterator i=tmp.begin();
    SimpleString tmpname;
    SimpleString tmpvalue;
    SimpleString currentSection;
    
findname:
    if (i == tmp.end()) {
        // We're done
        goto end;
    }
    switch (*i) {
    case '{':
        if (sections.insert(tmpname).second == false) {
            // There is already a section by that name !!
            sections.clear();
            variables.clear();
            return false;
        }
        currentSection = tmpname;
        tmpname.resize(0);
        goto insection;
        break;
    default:
        tmpname.append(&*i,1);
    }
    ++i;
    goto findname;
        
insection:
    if (i == tmp.end()) {
        sections.clear();
        variables.clear();
        return false;
    }
    switch (*i) {
    case '{':
        ++recursion; 
        break;
    case '}':
        --recursion;
        if (recursion == 0) {
            ++i;
            // We finished extracting the variables, don't keep those around
            currentSection.resize(0);
            goto findname;
        }
        break;
    default:
        goto variablename;
    }
    ++i;
    goto insection;
    
variablename:
    if (i == tmp.end()) {
            sections.clear();
            variables.clear();
        return false;
    }
    switch (*i) {
    case '{':
        // It was not a variable but the start of a new block
        goto insection;
    case '}':
            sections.clear();
            variables.clear();
        return false;
    case '=':
        if (tmpname.empty()) {
            sections.clear();
            variables.clear();
            return false;
        }
        ++i;
        goto variablevalue;
    default:
        tmpname.append(&*i,1);
        break;
    }
    ++i;
    goto variablename;    
    
variablevalue:
    if (i == tmp.end()) {
            sections.clear();
            variables.clear();
        return false;
    }
    switch (*i) {
    case '{':
            sections.clear();
            variables.clear();
        return false;
    case '}':
            sections.clear();
            variables.clear();
        return false;
    case ';':
        if (tmpvalue.empty()) {
            sections.clear();
            variables.clear();
            return false;
        }
        // We store the variable with the name of the section as a prefix
        buf.assign(currentSection);
        buf.append('/');
        buf.append(tmpname);
        variables.insert(VariableSet::value_type(buf, tmpvalue));
        tmpname.resize(0);
        tmpvalue.resize(0);
        ++i;
        goto insection;
    default:
        tmpvalue.append(i,1);
        break;
    }
    ++i;
    goto variablevalue;    
end:
    return true;
}

Config::Config(const SimpleString &sFileName) :
m_pVariables(NULL),
m_pSections(NULL),
m_sFileName(sFileName),
m_bLoaded(false)
{
}

Config::~Config()
{
    if (m_pVariables != NULL)
        delete static_cast<VariableSet *>(m_pVariables);
    if (m_pSections != NULL)
        delete static_cast<StringSet *>(m_pSections);
}

int Config::SetSection(const SimpleString &sName)
{
    if (!m_bLoaded) {
        m_bLoaded = true;
        m_pVariables = new VariableSet();
        m_pSections = new StringSet();
		std::fstream InputFile;
        bool result;
		InputFile.open(m_sFileName.c_str(), std::ios::in);
        if (InputFile.is_open() == 0) {
            result = false; 
        } else {
            result = preload(InputFile, 
                *(static_cast<VariableSet *>(m_pVariables)),
                *(static_cast<StringSet *>(m_pSections)));
            InputFile.close();
        }
        if (!result) {
            delete static_cast<VariableSet *>(m_pVariables);
            m_pVariables = NULL;
            delete static_cast<StringSet *>(m_pSections);
            m_pSections = NULL;
            return -1;
        }
    }
    if (m_pVariables == NULL) {
        return -1;
    }
    StringSet::const_iterator i = 
        ((StringSet*)m_pSections)->find(sName);
    if (i != ((StringSet*)m_pSections)->end() ) {
        m_sCurrentSection.assign(sName).append('/');
        return 0;
    } else {
        m_sCurrentSection.resize(0);
        return -1;
    }
}

long Config::GetByNameAsInteger(const SimpleString &sName, long lDefaut) const
{
    static SimpleString longName;
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        ((VariableSet *)m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) 
        return atol((*i).second.c_str());
    else {
        return lDefaut;
    }
}

const SimpleString &Config::GetByNameAsString(const SimpleString &sName, const SimpleString &sDefaut) const
{
    static SimpleString longName;
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        ((VariableSet *)m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) 
        return (*i).second;
    else {
        return sDefaut;
    }
}

double Config::GetByNameAsFloat(const SimpleString &sName, double fDefaut) const
{
    static SimpleString longName;
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        static_cast<VariableSet *>(m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) 
        return atof((*i).second.c_str());
    else {
        return fDefaut;
    }
}

Colour Config::GetByNameAsFloatOrColour(const SimpleString& sName, double fDefaut) const
{
    float fScalar = float(GetByNameAsFloat(sName, 0.0f)); 
    Vector vColor = { fScalar, fScalar, fScalar };
    vColor = GetByNameAsVector(sName, vColor);

	return Colour(vColor.x, vColor.y, vColor.z);
}

bool Config::GetByNameAsBoolean(const SimpleString &sName, bool bDefaut) const
{
    static SimpleString longName;
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        static_cast<VariableSet *>(m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) {
        return (*i).second.compare("true")==0;
    } else {
        return bDefaut;
    }
}

Vector Config::GetByNameAsVector(const SimpleString &sName, const Vector& vDefault) const
{
    Vector tempVecteur;
    static SimpleString longName; 
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        static_cast<VariableSet *>(m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) {
        int nbRead = sscanf(i->second.c_str(), "%f,%f,%f", &tempVecteur.x, &tempVecteur.y, &tempVecteur.z);
        if (nbRead != 3) {
            return vDefault;
        }
        
        return tempVecteur;
    } else {
        return vDefault;
    }
}

Point Config::GetByNameAsPoint(const SimpleString &sName, const Point& ptDefault) const
{
    Point tempPoint;
    static SimpleString longName; 
    longName.assign(m_sCurrentSection).append(sName);
    VariableSet::const_iterator i = 
        static_cast<VariableSet *>(m_pVariables)->find(longName);
    if (i != ((VariableSet *)m_pVariables)->end()) {
        int nbRead = sscanf(i->second.c_str(), "%f,%f,%f", &tempPoint.x, &tempPoint.y, &tempPoint.z);
        if (nbRead != 3) {
            return ptDefault;
        }
        
        return tempPoint;
    } else {
        return ptDefault;
    }
}

#pragma warning(pop)
