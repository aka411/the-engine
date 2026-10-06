plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "@ANDROID_PACKAGE_NAME@"
    compileSdk {
        version = release(37)
    }

    defaultConfig {
        applicationId = "@ANDROID_PACKAGE_NAME@"
        minSdk = 26 // was 24 , changed for adaptive icons , should consider 24 later
        targetSdk = 37
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        ndk {
            abiFilters.addAll(setOf("arm64-v8a", "x86_64"))
        }

        externalNativeBuild {
            cmake {
                cppFlags += "-std=c++20"
            }
        }
    }

    buildTypes {
        release {
            optimization {
                enable = false
            }
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("@APP_CMAKE_PATH@/CMakeLists.txt")
            version = "3.31.6"
        }
    }


    sourceSets {
       getByName("main") {

            assets.directories.add("@THE_ENGINE_APP_ASSETS_PATH@")
        }
    }


}

dependencies {
    implementation(libs.androidx.appcompat)
    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.games.activity)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.espresso.core)
    androidTestImplementation(libs.androidx.junit)
}