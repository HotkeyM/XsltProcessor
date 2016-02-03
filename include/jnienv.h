#ifndef JNIENV_H
#define JNIENV_H
#include "jni.h"
#include "string.h"

#include <QDebug>
#include <iostream>

template <typename T>
class Singleton
{
public:
    Singleton() = delete;

    template <typename... Args>
    static T* GetInstance(Args... args);

private:
    static T* instance;
};

template <typename T>
template <typename... Args>
T* Singleton<T>::GetInstance(Args... args)
{

    if (!instance)
    {
       instance = new T(args...);
    }
    return instance;
}

template<typename T> T* Singleton<T>::instance;



class Java
{
public:
    Java ()
    {
        qDebug () << "singleton constructor";

        JavaVMInitArgs vm_args;
        JavaVMOption options[2];

        options[0].optionString =
              "-Djava.class.path=c:/java/test2.jar";

        options[1].optionString = "-verbose:jni";

        memset((void*)&vm_args,0, sizeof(vm_args));
        vm_args.ignoreUnrecognized = JNI_FALSE;
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = options;


        // Construct a VM
        jint res = JNI_CreateJavaVM(&javaVM, (void **)&env, &vm_args);
    }

    Java (const char * jarPath)
    {
        JavaVMInitArgs vm_args;
        JavaVMOption options[2];

        char *buf = new char[strlen("-Djava.class.path=") + strlen (jarPath)];

        strcpy(buf, "-Djava.class.path=");

        options[0].optionString = strcat(buf, jarPath);

        options[1].optionString = "-verbose:jni";

        memset((void*)&vm_args,0, sizeof(vm_args));
        vm_args.ignoreUnrecognized = JNI_FALSE;
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = options;


        // Construct a VM
        jint res = JNI_CreateJavaVM(&javaVM, (void **)&env, &vm_args);
    }


    ~Java()
    {
        javaVM->DestroyJavaVM();
    }

    /////////////////////////////////////////////////////////
    // CallStaticMethod implemetation

    template <typename MethodType, typename... Args>
    MethodType CallStaticMethod(const char* className, const char* mname, Args... args)
    {

        qDebug() << "jniwrapper.h: javaWM=" << javaVM;
        qDebug() << "jniwrapper.h: javaEnv=" << env;


        const size_t arg_num = sizeof...(Args);
        std::string signatures[arg_num] = { GetType(args)... };

        std::string signature_string;
        signature_string.reserve(15);
        signature_string += "(";
        for (size_t i = 0; i < arg_num; ++i)
            signature_string += signatures[i];
        signature_string += ")";
        signature_string += GetTypeName<MethodType>();

        //JNIEnv *env = getEnv();

        qDebug() << "jniwrapper.h: signature staff done";

        JniClass clazz(env, className);

        qDebug() << "jniwrapper.h: clazz found";

        qDebug() << "class=" << clazz.get() << ";method_name=" << mname <<";signature=" << signature_string.c_str();

        jmethodID method = env->GetStaticMethodID(clazz.get(), mname, signature_string.c_str());

        qDebug() << "jniwrapper.h: method got " << method;


        return Impl<MethodType>::CallStaticMethod(env, clazz.get(), method, args...);
    }

    bool Exception()
    {
        bool res =  env->ExceptionCheck();
        env->ExceptionClear();
        return res;
    }

private:

    JavaVM *javaVM = nullptr;
    JNIEnv *env = nullptr;

    JNIEnv* getEnv()
    {
        JNIEnv *env = NULL;
        int status = javaVM->AttachCurrentThread((void**)&env, NULL);
        if (status < 0) {
            return NULL;
        }

        return env;
    }

    class JavaVoid
    {
        public:
        JavaVoid(){}
        static JavaVoid GetVoid()
        {
            return v;
        }

        private:
        static JavaVoid v;
    };

    class JniClass
    {
        JNIEnv* m_env;
        jclass m_class;
    public:
        JniClass(JNIEnv* env, const char* name)
            : m_env(env)
        {
            m_class = m_env->FindClass(name);
            qDebug() << "JniClass " << this << ": m_class=" << m_class;
        }

        ~JniClass()
        {
            if (m_class)
                m_env->DeleteLocalRef(m_class);
        }

        jclass get() { return m_class; }
    };

    /////////////////////////////////////////////////////////

    struct JObjectHolder
    {
        jobject jObject;
        JNIEnv* m_env;

        JObjectHolder()
            : m_env(nullptr)
        {}

        JObjectHolder(JNIEnv* env, jobject obj)
            : jObject(obj)
            , m_env(env)
        {}

        ~JObjectHolder()
        {
            if (jObject && m_env != nullptr)
            {
                m_env->DeleteLocalRef(jObject);
                qDebug() << "local ref " << jObject << " deleted";
            }
        }

        jobject detach()
        {
            jobject temp = jObject;
            jObject = NULL;
            return temp;
        }

        jobject get() { return jObject; }

        JNIEnv* getEnv() { return m_env; }

    };

    /////////////////////////////////////////////////////////

    struct JniHolder
    {
        jvalue val;
        JObjectHolder jObject;

        // move constructor
        JniHolder(JniHolder&& other)
            : jObject(other.jObject.getEnv(), jObject.detach())
            , val(other.val)
        {
            qDebug() << "JniHolder->move dtor";
        }

        // bool
        explicit JniHolder(JNIEnv *env, bool arg)
            : jObject(env, jobject())
        {
            val.z = arg;
        }

        // byte
        explicit JniHolder(JNIEnv *env, unsigned char arg)
            : jObject(env, jobject())
        {
            val.b = arg;
        }

        // char
        explicit JniHolder(JNIEnv *env, char arg)
            : jObject(env, jobject())
        {
            val.c = arg;
        }

        // short
        explicit JniHolder(JNIEnv *env, short arg)
            : jObject(env, jobject())
        {
            val.s = arg;
        }

        // int
        explicit JniHolder(JNIEnv *env, int arg)
            : jObject(env, jobject())
        {
            val.i = arg;
        }

        // long
        explicit JniHolder(JNIEnv *env, long arg)
            : jObject(env, jobject())
        {
            val.j = arg;
        }

        // float
        explicit JniHolder(JNIEnv *env, float arg)
            : jObject(env, jobject())
        {
            val.f = arg;
        }

        // double
        explicit JniHolder(JNIEnv *env, double arg)
            : jObject(env, jobject())
        {
            val.d = arg;
        }

        // string
        explicit JniHolder(JNIEnv *env, const char* arg)
            : jObject(env, env->NewStringUTF(arg))
        {
            val.l = jObject.get();
            //qDebug() << "new jstring from " << QString(arg) << "is:" << val.l;
        }

        // object
        explicit JniHolder(JNIEnv *env, jobject arg)
            : jObject(env, arg)
        {
            val.l = jObject.get();
        }

        ////////////////////////////////////////////////////////

        operator jvalue()
        {
            qDebug () << "static cast JNIHolder->jvalue";
            return val;

        }

        jvalue get() { return val; }
    };




    template <typename T>
    std::string GetTypeName();



    template <typename T>
    std::string GetType(const T&)
    {
        return GetTypeName<T>();
    }


    void GetTypeRecursive(std::string&)
    {   }

    template <typename T, typename... Args>
    void GetTypeRecursive(std::string& signatureString, T value, Args... args)
    {
        signatureString += GetTypeName<T>();
        GetTypeRecursive(signatureString, args...);
    }

    /////////////////////////////////////////////////////////
    // Impl class interface -> call all needed methods;
    //		Realizations in partial specializations

    template <typename MethodType>
    struct Impl
    {
        // for each type need concrete method in JNI
        //  int -> CallIntMethod
        //  void -> CallVoidMethod...
        template <typename... Args>
        static MethodType CallMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args);

        template <typename... Args>
        static void CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args);
    };  

};



/////////////////////////////////////////////////////////
// Impl<void>

template <>
struct Java::Impl <void>
{
    template <typename... Args>
    static void CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
    {
        const int size = sizeof...(args);
        if (size != 0)
        {
            //JniHolder holders[size] = { std::move(JniHolder(env, args))... };
            JniHolder holders[size] =  {JniHolder(env, args)...};
            jvalue vals[size];
            //for (size_t i = 0; i < size; ++i)
            //    vals[i] = static_cast<jvalue>(holders[i]);
            for (auto i = std::begin(holders); i !=std::end(holders); ++i)vals[std::distance(std::begin(holders),i)] = static_cast<jvalue>(*i);

            return env->CallStaticVoidMethodA(clazz, method, vals);
        }

        return env->CallStaticVoidMethod(clazz, method);
    }
};

/////////////////////////////////////////////////////////
// Impl<int>

template <>
struct Java::Impl <int>
{
    template <typename... Args>
    static int CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
    {
        const int size = sizeof...(args);
        if (size != 0)
        {

            JniHolder holders[size] =  {JniHolder(env, args)...};
            // массив холдеров - чтобы холдеры не разрушались после каста, как временные объекты
            // и не вызывали освобождение соответствующего ресурса

            jvalue vals[size];

            for (auto i = std::begin(holders); i !=std::end(holders); ++i)vals[std::distance(std::begin(holders),i)] = static_cast<jvalue>(*i);


            //std::cout << "vals"

            if (size > 0)
            {
                jvalue j;

                for(auto &i: vals) qDebug() << &i << "jvalue.l value = " << i.l;
            }


            return env->CallStaticIntMethodA(clazz, method, vals);
        }

        return env->CallStaticIntMethod(clazz, method);

    }
};

/////////////////////////////////////////////////////////
// Impl<float>

template <>
struct Java::Impl <float>
{
    template <typename... Args>
    static float CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
    {
        const int size = sizeof...(args);
        if (size != 0)
        {
            JniHolder holders[size] =  {JniHolder(env, args)...};

            jvalue vals[size];

            for (auto i = std::begin(holders); i !=std::end(holders); ++i)vals[std::distance(std::begin(holders),i)] = static_cast<jvalue>(*i);


            //jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
            return env->CallStaticFloatMethodA(clazz, method, vals);

        }

        return env->CallStaticFloatMethod(clazz, method);
    }
};

/////////////////////////////////////////////////////////
// Impl<std::string>

template <>
struct Java::Impl <std::string>
{
    template <typename... Args>
    static std::string CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
    {
        //qDebug() << "d";
        const int size = sizeof...(args);
        jstring jstr;
        if (size != 0)
        {
            //qDebug() << "dd";
            //jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
            JniHolder holders[size] =  {JniHolder(env, args)...};

            jvalue vals[size];

            for (auto i = std::begin(holders); i !=std::end(holders); ++i)vals[std::distance(std::begin(holders),i)] = static_cast<jvalue>(*i);

            jstr = (jstring)env->CallStaticObjectMethodA(clazz, method, vals);
            //qDebug() << "ddd";
            qDebug() << "Impl: env->CallStaticMethod returned jvalue jstr=" << jstr;
        }
        else
            jstr = (jstring)env->CallStaticObjectMethod(clazz, method);

        if (!jstr)
            return std::string();

        //qDebug() << "dddd";
        const char* chars = env->GetStringUTFChars(jstr, NULL);

        std::cout << "Impl: CallStaticMethod returned=" << chars << endl << std::flush;
        //qDebug() << "deb";

        std::string result;
        if (chars)
            result = chars;

        qDebug() << "result length = " << result.size();

        env->ReleaseStringUTFChars(jstr, chars);
        return result;
    }
};



template <>
inline std::string Java::GetTypeName<void>()
{
    qDebug() << "void* argument = 'V' specifier";
    return "V";
}

template <>
inline std::string Java::GetTypeName<bool>()
{
    return "Z";
}

template <>
inline std::string Java::GetTypeName<unsigned char>()
{
    return "B";
}

template <>
inline std::string Java::GetTypeName<char>()
{
    return "C";
}

template <>
inline std::string Java::GetTypeName<short>()
{
    return "S";
}

template <>
inline std::string Java::GetTypeName<int>()
{
    return "I";
}

template <>
inline std::string Java::GetTypeName<long>()
{
    return "J";
}

template <>
inline std::string Java::GetTypeName<float>()
{
    return "F";
}

template <>
inline std::string Java::GetTypeName<double>()
{
    return "D";
}

template <>
inline std::string Java::GetTypeName<const char*>()
{
    return "Ljava/lang/String;";
}

template <>
inline std::string Java::GetTypeName<std::string>()
{
    return "Ljava/lang/String;";
}

template <>
inline std::string Java::GetTypeName<jobject>()
{
    return "Ljava/lang/Object;";
}



#endif // JNIENV_H
