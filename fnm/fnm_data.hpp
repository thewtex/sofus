/**
 * @file   fnm_data.hpp
 * @author Jens Munk Hansen <jens.munk.hansen@gmail.com>
 * @date   Thu Jun  9 06:12:31 2016
 *
 * @brief Data structures containing data for field computation
 *
 *
 */

/*
 *  This file is part of SOFUS.
 *
 *  SOFUS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SOFUS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SOFUS.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <fnm/fnm_export.h>
#include <fnm/config.h>

#include <sps/cenv.h>
#include <sps/memory>
#include <sps/smath.hpp> // sps::point_t and sps::euler_t, now element_rect_t

#include <fnm/fnm_types.hpp> // FocusingType, element_rect_t

namespace fnm {

  /*! \brief ApertureData structure
   *
   * A struct containing data of the aperture
   *
   * @tparam T floating point type
   *
   */
#ifdef _MSC_VER
  template <class T>
  class ApertureData
#else
  template <class T>
  ALIGN16_BEGIN struct ALIGN16_END ApertureData
      // __attribute__((aligned(4*sizeof(T))))
#endif
  {
#ifdef _MSC_VER
  public:
#endif
    typedef sps::deleted_aligned_multi_array<sps::element_rect_t<T>, 2U> element_array;

    /// Next unique identifier to use
    static size_t nextID;

    /// Number of elements
    size_t m_nelements;

    /// Number of sub-elements per element
    size_t m_nsubelements;

    /// Number of positions, equals number of elements
    size_t m_npos;

    /// Elements
    element_array m_elements;

    /// Positions
    sps::deleted_aligned_array<sps::point_t<T> > m_pos;

    /// Apodization values
    sps::deleted_aligned_array<T> m_apodizations;

    /// Phases, range is (-pi;pi]
    sps::deleted_aligned_array<T> m_phases;

    /// Delays
    sps::deleted_aligned_array<T> m_delays;

    /// Rectangles
    sps::deleted_aligned_array<sps::rect_t<T> > m_rectangles;

    /// Boxes encapsulating sub-elements
    sps::deleted_aligned_array<sps::bbox_t<T> > m_boxes;

    /// Maximum half width or height (used for far field memory allocation)
    T m_h_xyz[3];

    /// Center frequency
    T m_f0;

    /// Focus point
    sps::point_t<T> m_focus;

    /// Focus point
    sps::point_t<T> m_center_focus;

    /// Focusing Type, Rayleigh or Pythagorean
    int m_focus_type;

    /// Apodization Type, Parametric, Hamming or Gaussian
    int m_apodization_type;

    /// F-number
    T m_fnumber;
    
    //@{  Internal state variables

    /// Validity of focus, if equal m_focus_type, phases are valid
    int m_focus_valid;

    /// Unique identifier
    size_t m_id;

    //@}
    /// Ctor
    ApertureData();

    /**
     * Set elements. Data are moved
     *
     * @param elements
     * @param nRows
     * @param nCols
     */
    void ElementsSet(element_array &&elements,
                     const size_t& nRows, const size_t& nCols) __restrict; /* pointer is unaliased */

    int ApodizationSet(sps::deleted_aligned_array<T> &&apodization,
                       const size_t nElements);

    int ApodizationSet(const sps::point_t<T>& direction, const T& depth, const ApodizationType& type);
    
    /**
     * Get reference to (sub)-elements
     *
     * @param nElements
     * @param nSubElements
     * @param elements double-pointer to const elements
     *
     * @return
     */
    int ElementsRefGet(size_t* nElements, size_t* nSubElements, const sps::element_rect_t<T>**& elements) const;

    /**
     * Get reference to apodizations
     *
     * @param nElements
     * @param apodizations pointer to const apodizations
     *
     * @return
     */
    int ApodizationsRefGet(size_t* nElements, const T*& apodizations) const;

    /**
     *
     *
     * @param nElements
     * @param delays
     *
     * @return
     */
    int DelaysRefGet(size_t* nElements, const T*& delays) const;

    /**
     * ElementExtentGet
     *
     * @param iElement
     * @param bbox Bounding box enclosing sub-elements for the i'th element
     */
    void ElementExtentGet(const size_t iElement, sps::bbox_t<T>& bbox) const;

    /**
     * Return extent of aperture in directions: x, y, and z
     *
     * @param bbox Bounding box enclosing aperture
     */
    void ExtentGet(sps::bbox_t<T>& bbox) const;

    /**
     * Total acoustic area
     *
     *
     * @return total area of elements
     */
    T AreaGet() const;

    /**
     * Dtor
     *
     *
     * @return
     */
    ~ApertureData();
  private:
    /**
     * Initialize basis vectors for each element
     *
     */
    void initVectors();

    /**
     * Initialize apodizations, positions, phases and delays
     *
     */
    void initElements();

    /**
     * Initialize rectangles used for Far field approximations
     *
     */
    void initRectangles();

    // Note: Deep copy is needed, if it is made copyable.
    ApertureData(const ApertureData& other) = delete;
    ApertureData& operator=(const ApertureData& other) = delete;
    ApertureData(ApertureData&& other) = delete;
    ApertureData& operator=(ApertureData&& other) = delete;
  };
}

/* Local variables: */
/* indent-tabs-mode: nil */
/* tab-width: 2 */
/* c-basic-offset: 2 */
/* End: */
