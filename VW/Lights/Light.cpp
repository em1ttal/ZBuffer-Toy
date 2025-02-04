#include "Light.hh"

Light::Light(vec3 Ia, vec3 Id, vec3 Is) {
    this->Ia = Ia;
    this->Id = Id;
    this->Is = Is;
}

Light::Light() {

}

vec3 Light::getIa() {
    return Ia;
}

vec3 Light::getId() {
    return Id;
}
vec3 Light::getIs() {
    return Is;
}

/**
 * @brief Light::setId
 * @param i
 */
void Light::setId(vec3 i) {
    // TO DO: Pràctica 2: A canviar a la fase 1
    this->Id = i;
}

/**
 * @brief Light::setLightPosition
 * @param v
 */
void Light::setLightPosition(vec4 v) {
    // TO DO: Pràctica 2: A canviar a la fase 1
    this->pos = v;
}

/**
 * @brief Light::setIa
 * @param value
 */
void Light::setIa(const vec3 &value)
{
   // TO DO: Pràctica 2: A canviar a la fase 1
    this->Ia = value;
}

/**
 * @brief Light::setIs
 * @param value
 */
void Light::setIs(const vec3 &value)
{
    // TO DO: Pràctica 2: A canviar a la fase 1
    this->Is = value;
}

/**
 * @brief Light::getCoeficients
 * @return
 */
vec3 Light::getCoeficients() const
{
    // TO DO: Pràctica 2: A canviar a la fase 1
       return vec3(a,b,c);
}

/**
 * @brief Light::setCoeficients
 * @param value
 */
void Light::setCoeficients(const vec3 &value)
{
   // TO DO: Pràctica 2: A canviar a la fase 1
    a = value[0];
    b = value[1];
    c = value[2];
}




void Light::read (const QJsonObject &json)
{
    if (json.contains("Ia") && json["Ia"].isArray()) {
        QJsonArray auxVec = json["Ia"].toArray();
        Ia[0] = auxVec[0].toDouble();
        Ia[1] = auxVec[1].toDouble();
        Ia[2] = auxVec[2].toDouble();
    }
    if (json.contains("Id") && json["Id"].isArray()) {
        QJsonArray auxVec = json["Id"].toArray();
        Id[0] = auxVec[0].toDouble();
        Id[1] = auxVec[1].toDouble();
        Id[2] = auxVec[2].toDouble();
    }
    if (json.contains("Is") && json["Is"].isArray()) {
        QJsonArray auxVec = json["Is"].toArray();
        Is[0] = auxVec[0].toDouble();
        Is[1] = auxVec[1].toDouble();
        Is[2] = auxVec[2].toDouble();
    }
}


//! [1]
void Light::write(QJsonObject &json) const
{
    QJsonArray auxArray;
    auxArray.append(Ia[0]);auxArray.append(Ia[1]);auxArray.append(Ia[2]);
    json["Ia"] = auxArray;

    QJsonArray auxArray2;
    auxArray2.append(Id[0]);auxArray2.append(Id[1]);auxArray2.append(Id[2]);
    json["Id"] = auxArray2;

    QJsonArray auxArray3;
    auxArray3.append(Is[0]);auxArray3.append(Is[1]);auxArray3.append(Is[2]);
    json["Is"] = auxArray3;
}
//! [1]

void Light::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Light:" << "\n";
    QTextStream(stdout) << indent << "Ia:\t" << Ia[0] << ", "<< Ia[1] << ", "<< Ia[2] << "\n";
    QTextStream(stdout) << indent << "Id:\t" << Id[0] << ", "<< Id[1] << ", "<< Id[2] << "\n";
    QTextStream(stdout) << indent << "Is:\t" << Is[0] << ", "<< Is[1] << ", "<< Is[2] << "\n";

}

