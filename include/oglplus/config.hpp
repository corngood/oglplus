/**
 *  @file oglplus/config.hpp
 *  @brief Compile-time configuration options
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_CONFIG_1107121519_HPP
#define OGLPLUS_CONFIG_1107121519_HPP

#ifndef OGLPLUS_NO_SITE_CONFIG
#include <oglplus/site_config.hpp>
#endif

#ifndef OGLPLUS_USE_GLEW
#define OGLPLUS_USE_GLEW 0
#endif

#ifndef OGLPLUS_USE_BOOST_CONFIG
#define OGLPLUS_USE_BOOST_CONFIG 0
#endif

#if OGLPLUS_USE_BOOST_CONFIG
#include <boost/config.hpp>
#endif

// ------- C++11 feature availability detection -------

#ifndef OGLPLUS_NO_VARIADIC_TEMPLATES
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#define OGLPLUS_NO_VARIADIC_TEMPLATES 1
#else
#define OGLPLUS_NO_VARIADIC_TEMPLATES 0
#endif
#endif

#ifndef OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX
#ifdef BOOST_NO_UNIFIED_INITIALIZATION_SYNTAX
#define OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX 1
#else
#define OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX 0
#endif
#endif

#ifndef OGLPLUS_NO_INITIALIZER_LISTS
#ifdef BOOST_NO_INITIALIZER_LISTS
#define OGLPLUS_NO_INITIALIZER_LISTS 1
#else
#define OGLPLUS_NO_INITIALIZER_LISTS 0
#endif
#endif

#ifndef OGLPLUS_NO_DEFAULTED_FUNCTIONS
#ifdef BOOST_NO_DEFAULTED_FUNCTIONS
#define OGLPLUS_NO_DEFAULTED_FUNCTIONS 1
#else
#define OGLPLUS_NO_DEFAULTED_FUNCTIONS 0
#endif
#endif

#ifndef OGLPLUS_NO_DELETED_FUNCTIONS
#ifdef BOOST_NO_DELETED_FUNCTIONS
#define OGLPLUS_NO_DELETED_FUNCTIONS 1
#else
#define OGLPLUS_NO_DELETED_FUNCTIONS 0
#endif
#endif

#ifndef OGLPLUS_NO_NULLPTR
#ifdef BOOST_NO_NULLPTR
#define OGLPLUS_NO_NULLPTR 1
#else
#define OGLPLUS_NO_NULLPTR 0
#endif
#endif

// ------- C++11 feature availability detection -------

#if OGLPLUS_NO_NULLPTR
#define nullptr 0
#endif

// define GLAPIENTRY
#ifdef GLAPIENTRY
#undef GLAPIENTRY
#endif
// borrowed from glew.h which does define GLAPIENTRY properly
// at the beginning but undefs in at the end of the header
#if defined(__MINGW32__) || defined(__CYGWIN__)
#  define GLAPIENTRY __stdcall
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#  define GLAPIENTRY __stdcall
#else
#  define GLAPIENTRY
#endif


/** @defgroup compile_time_config Compile-time configuration
 *
 *  This section describes compile-time preprocessor symbols that
 *  can be used to configure several aspects of @OGLplus.
 */


#ifndef OGLPLUS_DOCUMENTATION_ONLY
#define OGLPLUS_DOCUMENTATION_ONLY 0
#endif

#ifndef OGLPLUS_DONT_TEST_OBJECT_TYPE
/// Compile-time switch disabling the texting of object type on construction
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_DONT_TEST_OBJECT_TYPE 1
#endif

#ifndef OGLPLUS_NO_OBJECT_DESCS
/// Compile-time switch disabling textual object descriptions
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_NO_OBJECT_DESCS 0
#endif

#ifndef OGLPLUS_NO_ENUM_VALUE_NAMES
/// Compile-time switch disabling the functions returning enumerated value names
/** Setting this preprocessor symbol to a nonzero value causes that
 *  the @c EnumValueName(Enum) functions always return an empty string.
 *  When set to zero these functions return a textual name of an enumerated
 *  value passed as argument.
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_NO_ENUM_VALUE_NAMES 0
#endif

#ifndef OGLPLUS_CUSTOM_ERROR_HANDLING
/// Compile-time switch enabling customized @ref error_handling
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_CUSTOM_ERROR_HANDLING 0
#endif


// Configuration options related to ErrorInfo


#ifndef OGLPLUS_ERROR_INFO_NO_GL_SYMBOL
/// Compile-time switch disabling the ErrorGLSymbol attribute of ErrorInfo
/**
 *  @see ErrorGLSymbol()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_GL_SYMBOL 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_FILE
/// Compile-time switch disabling the ErrorFile attribute of ErrorInfo
/**
 *  @see ErrorFile()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_FILE 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_LINE
/// Compile-time switch disabling the ErrorLine attribute of ErrorInfo
/**
 *  @see ErrorLine()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_LINE 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_FUNC
/// Compile-time switch disabling the ErrorFunc attribute of ErrorInfo
/**
 *  @see ErrorFunc()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_FUNC 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_CLASS_NAME
/// Compile-time switch disabling the ErrorClassName attribute of ErrorInfo
/**
 *  @see ErrorClassName()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_CLASS_NAME 0
#endif

#if OGLPLUS_NO_ENUM_VALUE_NAMES
#ifdef OGLPLUS_ERROR_INFO_NO_BIND_TARGET
#undef OGLPLUS_ERROR_INFO_NO_BIND_TARGET
#endif
#define OGLPLUS_ERROR_INFO_NO_BIND_TARGET 1
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_BIND_TARGET
/// Compile-time switch disabling the ErrorBindTarget attribute of ErrorInfo
/**
 *  @see ErrorBindTarget()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_BIND_TARGET 0
#endif

#if OGLPLUS_NO_OBJECT_DESCS
#ifdef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
#undef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
#endif
#define OGLPLUS_ERROR_INFO_NO_OBJECT_DESC 1
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
/// Compile-time switch disabling the ErrorObjectDescription attribute of ErrorInfo
/**
 *  @see ErrorObjectDescription()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_OBJECT_DESC 0
#endif

#endif // include guard
