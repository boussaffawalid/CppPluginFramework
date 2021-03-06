/* This file is part of C++ Plugin Framework.
 *
 * C++ Plugin Framework is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * C++ Plugin Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with C++ Plugin
 * Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file
 *
 * Contains a class that holds a plugin's config
 */

// Own header
#include <CppPluginFramework/PluginConfig.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/Validation.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

PluginConfig::PluginConfig(const QString &filePath,
                           const VersionInfo &version,
                           const QList<PluginInstanceConfig> &instanceConfigs)
    : m_filePath(filePath),
      m_version(version),
      m_minVersion(),
      m_maxVersion(),
      m_instanceConfigs(instanceConfigs)
{
}

// -------------------------------------------------------------------------------------------------

PluginConfig::PluginConfig(const QString &filePath,
                           const VersionInfo &minVersion,
                           const VersionInfo &maxVersion,
                           const QList<PluginInstanceConfig> &instanceConfigs)
    : m_filePath(filePath),
      m_version(),
      m_minVersion(minVersion),
      m_maxVersion(maxVersion),
      m_instanceConfigs(instanceConfigs)
{
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isValid() const
{
    // Check file path
    if (!Validation::validateFilePath(m_filePath))
    {
        return false;
    }

    // Check version info
    if (isExactVersion())
    {
        if (!m_version.isValid())
        {
            return false;
        }
    }
    else
    {
        if (!VersionInfo::isRangeValid(m_minVersion, m_maxVersion))
        {
            return false;
        }
    }

    // At least one plugin instance is required
    if (m_instanceConfigs.isEmpty())
    {
        return false;
    }

    // Check individual instances if it is valid
    for (const PluginInstanceConfig &instanceConfig : m_instanceConfigs)
    {
        if (!instanceConfig.isValid())
        {
            return false;
        }
    }

    // Check if this plugin config contains multiple instances with the same name
    for (const PluginInstanceConfig &instanceConfig : m_instanceConfigs)
    {
        int count = 0;

        for (const PluginInstanceConfig &item : m_instanceConfigs)
        {
            if (item.name() == instanceConfig.name())
            {
                count++;

                if (count > 1)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isExactVersion() const
{
    return ((!m_version.isNull()) && m_minVersion.isNull() && m_maxVersion.isNull());
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isVersionRange() const
{
    return (m_version.isNull() && (!m_minVersion.isNull()) && (!m_maxVersion.isNull()));
}

// -------------------------------------------------------------------------------------------------

QString PluginConfig::filePath() const
{
    return m_filePath;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::version() const
{
    return m_version;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setVersion(const VersionInfo &version)
{
    m_version = version;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::minVersion() const
{
    return m_minVersion;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setMinVersion(const VersionInfo &minVersion)
{
    m_minVersion = minVersion;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::maxVersion() const
{
    return m_maxVersion;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setMaxVersion(const VersionInfo &maxVersion)
{
    m_maxVersion = maxVersion;
}

// -------------------------------------------------------------------------------------------------

QList<PluginInstanceConfig> PluginConfig::instanceConfigs() const
{
    return m_instanceConfigs;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setInstanceConfigs(const QList<PluginInstanceConfig> &instanceConfigs)
{
    m_instanceConfigs = instanceConfigs;
}

// -------------------------------------------------------------------------------------------------

bool operator==(const PluginConfig &left, const PluginConfig &right)
{
    return ((left.filePath() == right.filePath()) &&
            (left.version() == right.version()) &&
            (left.minVersion() == right.minVersion()) &&
            (left.maxVersion() == right.maxVersion()) &&
            (left.instanceConfigs() == right.instanceConfigs()));
}

}
