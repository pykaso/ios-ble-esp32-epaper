import MobileCoreServices
import UIKit

extension UIImage {
    func toJpegData(compressionQuality: CGFloat, hasAlpha: Bool = true, orientation: Int = 6) -> Data? {
        guard cgImage != nil else { return nil }
        let options: NSDictionary = [
            kCGImagePropertyOrientation: orientation,
            kCGImagePropertyHasAlpha: hasAlpha,
            kCGImageDestinationLossyCompressionQuality: compressionQuality
        ]
        return toData(options: options, type: .jpeg)
    }

    func toData(options: NSDictionary, type: ImageType) -> Data? {
        guard cgImage != nil else { return nil }
        return toData(options: options, type: type.value)
    }

    // about properties: https://developer.apple.com/documentation/imageio/1464962-cgimagedestinationaddimage
    func toData(options: NSDictionary, type: CFString) -> Data? {
        guard let cgImage = cgImage else { return nil }
        return autoreleasepool { () -> Data? in
            let data = NSMutableData()
            guard let imageDestination = CGImageDestinationCreateWithData(data as CFMutableData, type, 1, nil) else { return nil }
            CGImageDestinationAddImage(imageDestination, cgImage, options)
            CGImageDestinationFinalize(imageDestination)
            return data as Data
        }
    }

    // https://developer.apple.com/documentation/mobilecoreservices/uttype/uti_image_content_types
    enum ImageType {
        case image // abstract image data
        case jpeg // JPEG image
        case jpeg2000 // JPEG-2000 image
        case tiff // TIFF image
        case pict // Quickdraw PICT format
        case gif // GIF image
        case png // PNG image
        case quickTimeImage // QuickTime image format (OSType 'qtif')
        case appleICNS // Apple icon data
        case bmp // Windows bitmap
        case ico // Windows icon data
        case rawImage // base type for raw image data (.raw)
        case scalableVectorGraphics // SVG image
        case livePhoto // Live Photo

        var value: CFString {
            switch self {
            case .image: return kUTTypeImage
            case .jpeg: return kUTTypeJPEG
            case .jpeg2000: return kUTTypeJPEG2000
            case .tiff: return kUTTypeTIFF
            case .pict: return kUTTypePICT
            case .gif: return kUTTypeGIF
            case .png: return kUTTypePNG
            case .quickTimeImage: return kUTTypeQuickTimeImage
            case .appleICNS: return kUTTypeAppleICNS
            case .bmp: return kUTTypeBMP
            case .ico: return kUTTypeICO
            case .rawImage: return kUTTypeRawImage
            case .scalableVectorGraphics: return kUTTypeScalableVectorGraphics
            case .livePhoto: return kUTTypeLivePhoto
            }
        }
    }

    var monochrome: UIImage? {
        let context = CIContext(options: nil)
        let currentFilter = CIFilter(name: "CIColorMonochrome")
        currentFilter?.setValue(CIImage(image: self), forKey: kCIInputImageKey)
        guard let output = currentFilter?.outputImage,
              let cgImage = context.createCGImage(output, from: output.extent) else {
            print("Failed to create output image")
            return nil
        }
        return UIImage(cgImage: cgImage, scale: scale, orientation: imageOrientation)
    }

    var pixelData: [Pixel] {
        let bmp = cgImage!.dataProvider!.data
        var data: UnsafePointer<UInt8> = CFDataGetBytePtr(bmp)
        var r, g, b, a: UInt8
        var pixels = [Pixel]()

        for row in 0 ..< Int(size.width) {
            for col in 0 ..< Int(size.height) {
                r = data.pointee
                data = data.advanced(by: 1)
                g = data.pointee
                data = data.advanced(by: 1)
                b = data.pointee
                data = data.advanced(by: 1)
                a = data.pointee
                data = data.advanced(by: 1)
                pixels.append(Pixel(r: r, g: g, b: b, a: a, row: row, col: col))
            }
        }
        return pixels
    }
}
