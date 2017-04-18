solution "easyrender"
   language "C++"
   location "project"
   targetdir "build/release"

	buildoptions({"/Qpar", "/Qfast_transcendentals", "/GL", "/Ox", "/Gm", "/MP", "/GR-", "/MD", "/Gy", "/Gw"})
   linkoptions { "/OPT:REF", "/OPT:ICF", "/LTCG"}
   
		 
   flags { "Optimize", "NoMinimalRebuild", "NoFramePointer", "EnableSSE2", "FloatFast", "NoBufferSecurityCheck"}
   characterset ("MBCS")

   if os.is("linux") or os.is("macosx") then
      buildoptions {"-m32 -fPIC -ldl -lstdc++"}
      linkoptions  {"-m32 -fPIC -ldl -lstdc++"}
   end

   vpaths {
      ["Header Files/*"] = "src/**.h",
      ["Source Files/*"] = "src/**.cpp",
   }

   kind "SharedLib"

   configurations { "Debug", "Release" }

   files { "src/**.h", "src/**.cpp" }
   
   includedirs {"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include" }
   libdirs {"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86", "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64" }
   
   -- A project defines one build target
   project "easyrender"
      targetname "easyrender"

      configuration "Release"
         defines { "NDEBUG", "_GENERIC" }
		 
      configuration "Debug"
         defines { "DEBUG", "_GENERIC" }
         flags { "Symbols"}
         targetdir "build/debug"