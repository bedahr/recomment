#include "numericalrelationship.h"
#include "offer.h"
#include <QDebug>

/// todo: weigh result with a "distance" from the constraint boundary
float NumericalRelationship::utility(const Offer& offer) const
{
    const Attribute* offerAttribute = offer.getAttribute(m_name);
    //qDebug() << "Offer attribute of name " << m_name << offerAttribute;
    if (!offerAttribute)
        return 1.0; // we don't know -> no violation

    const NumericalAttribute* numericalOfferAttribute = dynamic_cast<const NumericalAttribute*>(offerAttribute);
    if (!numericalOfferAttribute)
        return 0; // wrong type

    float baseUtility = Relationship::utility(offer);

    if (m_type & Relationship::SmallerThan &&
            *numericalOfferAttribute > static_cast<const NumericalAttribute&>(m_attribute))
        baseUtility = 0;
    if (m_type & Relationship::LargerThan &&
            *numericalOfferAttribute < static_cast<const NumericalAttribute&>(m_attribute))
        baseUtility = 0;

    //qDebug() << "Returning: " << baseUtility;
    return baseUtility;
}
