//! @file

#ifndef VIEWER_TYPE_HPP
#define VIEWER_TYPE_HPP

#include <osg/ref_ptr>
#include <osg/Drawable>

namespace Viewer {

typedef osg::ref_ptr<osg::Drawable> DrawablePtr;
typedef std::vector<DrawablePtr> DrawablePtrArray;

}

#endif
