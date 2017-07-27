
local BuilPath = "Build/" .. _ACTION  --工程路径
local ObjPath = "../ObjOut/" .. _ACTION  -- 编译中间文件输出目录
local OutPath = "%{cfg.buildcfg}/%{cfg.platform}"

workspace "Server"
	configurations {
		"Debug", 
		"Release" 
	}--项目配置类型
    platforms { "X32", "X64"}
    location(BuilPath)--解决方案目录

    flags {
		"C++11", 
		"MultiProcessorCompile"
	}
	
	filter "configurations:Debug" --编译配置
		defines { "DEBUG" } --预编译宏
		flags { "Symbols" } -- 生成pdb文件

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On" --开启优化选项
			
	group "CoreLib"
		
		project "LibNet"
			kind "StaticLib" 
			language "C++" 
			targetdir("Lib/"..OutPath) --生成文件目录
			location(BuilPath.."/CoreLib/LibNet") --工程目录
			objdir(ObjPath.."/CoreLib/LibNet/%{cfg.buildcfg}")
			includedirs {
				--"CoreLib/LibNet"
				"CoreLib/LibNet/networking-ts-impl/include",
				"CoreLib/LibNet/Internal",
			}
			files { 
				"CoreLib/LibNet/**.h",
				"CoreLib/LibNet/**.cpp",
				--"CoreLib/LibNet/Internal/*.h",
				--"CoreLib/LibNet/Internal/*.cpp",
			}
			removefiles{"*/networking-ts-impl/*"}
			defines{"_WIN32_WINNT=0x0501"}
			
		project "LibServer"
			kind "StaticLib" 
			language "C++" 
			targetdir("Lib/"..OutPath) --生成文件目录
			location(BuilPath.."/CoreLib/LibServer") --工程目录
			objdir(ObjPath.."/CoreLib/LibServer/%{cfg.buildcfg}")
			links{"LibNet"}
			includedirs {
				"CoreLib/LibNet",
				"Common/",
			}
			files { 
				"Common/Sdp/*.h",
				"CoreLib/LibServer/**.h",
				"CoreLib/LibServer/**.cpp" 
			}
		