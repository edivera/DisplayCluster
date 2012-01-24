#ifndef PIXEL_STREAM_CONTENT_H
#define PIXEL_STREAM_CONTENT_H

#include "Content.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

class PixelStreamContent : public Content {

    public:
        PixelStreamContent(std::string uri = "") : Content(uri) { }

        void getFactoryObjectDimensions(int &width, int &height);

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int)
        {
            // serialize base class information
            ar & boost::serialization::base_object<Content>(*this);
        }

        void renderFactoryObject(float tX, float tY, float tW, float tH);
};

#endif
