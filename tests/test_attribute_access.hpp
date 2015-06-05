#ifndef BLUETOE_TESTS_TEST_ATTRIBUTE_ACCESS_HPP
#define BLUETOE_TESTS_TEST_ATTRIBUTE_ACCESS_HPP

#include <bluetoe/attribute.hpp>

template < typename Char >
class access_attributes : public Char, public bluetoe::details::client_characteristic_configurations< Char::number_of_client_configs >
{
public:
    std::pair< bool, bluetoe::details::attribute > find_attribute_by_type( std::uint16_t type )
    {
        for ( int index = 0; index != this->number_of_attributes; ++index )
        {
            const bluetoe::details::attribute value_attribute = this->template attribute_at< 0 >( index );

            if ( value_attribute.uuid == type )
            {
                return std::make_pair( true, value_attribute );
            }
        }

        return std::pair< bool, bluetoe::details::attribute >( false, bluetoe::details::attribute{} );
    }

    bluetoe::details::attribute attribute_by_type( std::uint16_t type )
    {
        for ( int index = 0; index != this->number_of_attributes; ++index )
        {
            const bluetoe::details::attribute value_attribute = this->template attribute_at< 0 >( index );

            if ( value_attribute.uuid == type )
            {
                return value_attribute;
            }
        }

        BOOST_REQUIRE( !"Type not found" );

        return bluetoe::details::attribute{ 0, 0 };
    }

    void compare_characteristic_at( const std::initializer_list< std::uint8_t >& input, std::size_t index )
    {
        compare_characteristic_impl( input, this->template attribute_at< 0 >( index ) );
    }

    void compare_characteristic( const std::initializer_list< std::uint8_t >& input, std::uint16_t type )
    {
        compare_characteristic_impl( input, attribute_by_type( type ) );
    }

private:
    void compare_characteristic_impl( const std::initializer_list< std::uint8_t >& input, const bluetoe::details::attribute& value_attribute )
    {
        const std::vector< std::uint8_t > values( input );

        std::uint8_t buffer[ 1000 ];
        auto read = bluetoe::details::attribute_access_arguments::read( buffer, 0, this->client_configurations() );

        BOOST_REQUIRE( bluetoe::details::attribute_access_result::success == value_attribute.access( read, 1 ) );
        BOOST_REQUIRE_EQUAL_COLLECTIONS( values.begin(), values.end(), &read.buffer[ 0 ], &read.buffer[ read.buffer_size ] );
    }
};


#endif
