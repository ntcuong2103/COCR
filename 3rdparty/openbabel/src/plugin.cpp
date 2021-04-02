/**********************************************************************
plugin.cpp - facilitates construction of plugin classes

Copyright (C) 2007 by Chris Morley

This file is part of the Open Babel project.
For more information, see <http://openbabel.org/>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
***********************************************************************/

#include <openbabel/babelconfig.h>
#include <openbabel/plugin.h>
#include <openbabel/oberror.h>

#include <iterator>

using namespace std;
namespace OpenBabel {

    OBPlugin::PluginMapType &OBPlugin::GetTypeMap(const char *PluginID) {
        PluginMapType::iterator itr;

        // Make sure the plugins are loaded
        if (AllPluginsLoaded == 0) {
            OBPlugin::LoadAllPlugins();
        }

        itr = PluginMap().find(PluginID);
        if (itr != PluginMap().end())
            return itr->second->GetMap();
        return PluginMap();//error: type not found; return plugins map
    }

    int OBPlugin::AllPluginsLoaded = 0;

    void OBPlugin::LoadAllPlugins() {
        int count = 0;

        count = 1; // Avoid calling this function several times


        // Status have to be updated now
        AllPluginsLoaded = count;

        // Make instances for plugin classes defined in the data file.
        // This is hook for OBDefine, but does nothing if it is not loaded
        // or if plugindefines.txt is not found.
        OBPlugin *pdef = OBPlugin::GetPlugin("loaders", "define");
        if (pdef) {
            static vector<string> vec(3);
            vec[1] = string("define");
            vec[2] = string("plugindefines.txt");
            pdef->MakeInstance(vec);
        }

        return;
    }

    OBPlugin *OBPlugin::BaseFindType(PluginMapType &Map, const char *ID) {
        // Make sure the plugins are loaded
        if (AllPluginsLoaded == 0) {
            OBPlugin::LoadAllPlugins();
        }

        if (!ID || !*ID)
            return nullptr;
        PluginMapType::iterator itr = Map.find(ID);
        if (itr == Map.end())
            return nullptr;
        else
            return itr->second;
    }

    OBPlugin *OBPlugin::GetPlugin(const char *Type, const char *ID) {
        if (Type != nullptr)
            return BaseFindType(GetTypeMap(Type), ID);

        // Make sure the plugins are loaded
        if (AllPluginsLoaded == 0) {
            OBPlugin::LoadAllPlugins();
        }

        //When Type==NULL, search all types for matching ID and stop when found
        PluginMapType::iterator itr;
        for (itr = PluginMap().begin(); itr != PluginMap().end(); ++itr) {
            OBPlugin *result = BaseFindType(itr->second->GetMap(), ID);
            if (result)
                return result;
        }
        return nullptr; //not found
    }

    bool OBPlugin::ListAsVector(const char *PluginID, const char *param, vector<string> &vlist) {
        PluginMapType::iterator itr;
        bool ret = true;

        // Make sure the plugins are loaded
        if (AllPluginsLoaded == 0) {
            LoadAllPlugins();
        }

        if (PluginID) {
            if (*PluginID != 0 && strcmp(PluginID, "plugins")) {
                //List the sub classes of the specified type
                itr = PluginMap().find(PluginID);
                if (itr != PluginMap().end()) {
                    bool onlyIDs = param != nullptr && strstr(param, "ids") != nullptr;
                    //Get map of plugin type (like OBFingerprint) and output its contents
                    PluginMapType Map = itr->second->GetMap();
                    for (itr = Map.begin(); itr != Map.end(); ++itr) {
                        if (*(itr->first) == '_')//no listing when ID starts with '_'
                            continue;
                        if (onlyIDs)
                            vlist.push_back(itr->first);
                        else {
                            string txt;
                            if ((itr->second)->Display(txt, param, itr->first))
                                vlist.push_back(txt);
                        }
                    }
                    return true;
                }
                ret = false; //asked for a type not available; provide plugin types instead
            }
        }
        //List the plugin types
        for (itr = PluginMap().begin(); itr != PluginMap().end(); ++itr)
            vlist.push_back(itr->first);
        return ret;
    }

    void OBPlugin::List(const char *PluginID, const char *param, ostream *os) {
        vector<string> vlist;
        if (!ListAsVector(PluginID, param, vlist))
            *os << PluginID << " is not a recognized plugin type. Those with instances of sub-types loaded are:"
                << endl;
        copy(vlist.begin(), vlist.end(), std::ostream_iterator<string>(*os, "\n"));
    }

    string OBPlugin::ListAsString(const char *PluginID, const char *param) {
        stringstream ss;
        List(PluginID, param, &ss);
        return ss.str();
    }

    string OBPlugin::FirstLine(const char *txt) {
        string stxt(txt);
        string::size_type pos = stxt.find('\n');
        if (pos == string::npos)
            return stxt;
        else
            return stxt.substr(0, pos);
    }

//Default version
    bool OBPlugin::Display(string &txt, const char *param, const char *ID) {
        //Use the provided ID if possible.
        if (ID)
            txt = ID;
        else
            txt = GetID();
        txt += "    ";// was '\t'; but caused problems in GUI menu
        if (param && !strcasecmp(param, "verbose")) {
            txt += Description();
            txt += '\n';
        } else
            txt += FirstLine(Description());
        return true;
    }

/**
  /// @addtogroup plugins Plugins
  ///@{
 @page plugins Plugins
 Plugins are a way of extending OpenBabel without having to alter any of the
 existing code. They may be built as shared libraries (DLLs with an extension
 .obf or .so files in a specified location) and distributed separately, but
 plugin classes can also be in the main code. In both cases they are discovered
 at startup when a global instance of the plugin class is instantiated. It iss
 registered by its constructor and is added to a static record of all the
 plugins of its particular type that are currently loaded.

 There are two levels of plugin. The top layer (at the time of writing) are:
  formats descriptors fingerprints forcefields charges ops loaders
 but additional types can be added without disturbing the main API. At runtime
   obabel -L
 will list the top level of plugins. They typically are abstract classes with
 virtual functions that define an interface for that type. Classes derived
 from these are the second layer of plugins, and can be listed at runtime like,
 for instance:
   obabel -L formats cml
 where formats is the top level of plugin and cml is the id of a derived class
 of this type.

 The top level of plugins will usually have their interfaces declared in header
 files compiled with the main API. The second level of plugin will typically
 not be known to the API at compile time, usually will not have a header file
 and must be accessed indirectly, to allow for the possibility that they may
 not be loaded:
 @code
   OBOp* pOp = OBOp::FindType("gen3D");
   if(!pOp)
     ...report error
   pOp->Do(mol);
 @endcode
 This retrieves the global instance of the plugin. This is usually adequate but
 making a new instance may be appropriate in some cases.

 Instances of some plugin classes can be constructed at startup from information
 in a text file and used in the same way as those defined in code. See OBDefine.
 This is appropriate for some classes that differ only by the datafile or
 SMARTS strings they use.
*/
///@}
}//namespace

//! \file plugin.cpp
//! \brief Simplify 'plugin' classes to be discovered and/or loaded at runtime
